#include "../include/Plazza.hpp"
#include "../include/Kitchen.hpp"
#include <ctime>
#include <chrono>
#include <mutex>
#include <numeric>
#include <unistd.h>
#include "../include/Regina.hpp"
#include "../include/Cook.hpp"
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>

Kitchen::Kitchen(double multiplier, size_t cooks, size_t restock, int kNumber) :
    _multiplier(multiplier), _ncooks(cooks), _restock(restock), _id(kNumber)
{
    Cook *cook;
    this->_ingredients = new Stock();
    this->_ordersNow = 0;
    this->_socket = initControlSocket();

    for (size_t i = 0; i != _ncooks; i++)
    {
        _cookStatus.push_back(false);
    }
    for (size_t i = 0; i != _ncooks; i++)
    {
        cook = new Cook(this, this->_multiplier, i, _id);
        this->_cooks.push_back(std::thread(&Cook::Run, cook, std::ref(_orders), std::ref(_finOrders), std::ref(p_mutex), std::ref(_cookStatus)));
    }
    dprintf(this->_socket, "%d\r\n", AVAILABLE);
}

int Kitchen::initControlSocket()
{
    int op = 1;
    int s;
    struct sockaddr_in addr;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket()");
        exit(84);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect()");
        exit(84);
    }
    return (s);
}

bool Kitchen::isFull(void)
{
    return (this->_ncooks * 2) > (this->_ordersNow + 1);
}

Kitchen::~Kitchen()
{
    printf("kitchen CLOSED %d\n", (int)getpid());
}

void Kitchen::restock()
{
    this->_ingredientlock.lock();
    this->_ingredients->addStock();
    this->_ingredientlock.unlock();
}

void Kitchen::run()
{
    auto addTimer = std::chrono::high_resolution_clock::now();
    bool dying = false;
    fd_set fds;
    struct timeval tv;
    bool status = true;

    tv.tv_sec = 0;
    tv.tv_usec = 0; 
    errno = 0;
    while (1)
    {
        double elapsedAddTime = double(std::chrono::duration_cast<std::chrono::milliseconds> (addTimer - \
          std::chrono::high_resolution_clock::now()).count());
        if (-elapsedAddTime > this->_restock)
        {
            this->restock();
            addTimer = std::chrono::high_resolution_clock::now();
        }
        FD_ZERO(&fds);
        FD_SET(_socket, &fds);
        if ((select(_socket + 1, &fds, NULL, NULL, &tv)) < 0 && (errno != EINTR))
        {
            perror("client select()");
            exit(84);
        }
        if (FD_ISSET(_socket, &fds))
            processMsg();
        if (_finOrders.size() != 0)
        {
            char *str = "";
            APizza *pizz = _finOrders.top();
            _finOrders.pop();
            asprintf(&str, "%d;%d", pizz->getType(), pizz->getSize());
            sendMsgSocket(FIN_ORDER, str, _socket);
        }
        if (_orders.size() >= this->_ncooks && status == true)
        {
            sendMsgSocket(UNAVAILABLE, NULL, _socket);
            status = false;
        }
        else if (_orders.size() < this->_ncooks && status == false)
        {
            sendMsgSocket(AVAILABLE, NULL, _socket);
            status = true;
        }
    }
}

void Kitchen::setFinPizza(APizza *pizz)
{
    this->_finOrders.push(pizz);
}

void Kitchen::isAvailable()
{
    if (_orders.size() >= this->_ncooks * 2)
        sendMsgSocket(UNAVAILABLE, NULL, _socket);
    else
        sendMsgSocket(AVAILABLE, NULL, _socket);
}

void Kitchen::addOrder(char **toks)
{

    APizza *order = makeOrder(getPizzaTypeByInt(toks[1]), getPizzaSizeByInt(toks[2]));
    if (order == NULL)
        return;
    std::lock_guard<std::mutex> guard(p_mutex);
    _orders.push(order);
}

void Kitchen::processMsg()
{
    char *msg;
    char *buf[1];

    if (recv(_socket, buf, sizeof(buf), \
                MSG_PEEK | MSG_DONTWAIT) == 0)
    {
        std::cout << "error exiting kitchen" << this->_id << std::endl;
        exit(0);
    }
    if ((msg = readMsgSocket(_socket)) == NULL)
        return;
    char **toks = sepTok(msg, (char *)(const char *)";\r\n");
    if (toks == NULL || toks[0] == NULL)
        return;
    switch (atoi(toks[0]))
    {
        case IS_AVAILABLE:
            this->isAvailable();
            break;
        case NEW_ORDER:
            this->addOrder(toks);
            break;
        case CLOSE_KITCHEN:
            exit(0);
        case STATUS_KITCHEN:
            this->status();
            break;
        default:
            break;
    }
}

void Kitchen::status()
{
    int currOccupancy = 0;
    for (size_t i = 0; i !=  _cookStatus.size(); i++)
    {
        if (_cookStatus[i] == true)
            currOccupancy++;
    }
    char *str = "";
    char *strStock = _ingredients->getStock();

    if (strStock == NULL)
        return;
    asprintf(&str, "%d;%s", currOccupancy, strStock);
    if (str == NULL)
        return;
    sendMsgSocket(STATUS_KITCHEN, str, _socket);
}

void Kitchen::lockPizzas()
{
    this->_pizzalock.lock();
}

void Kitchen::unlockPizzas()
{
    this->_pizzalock.unlock();
}

void Kitchen::lockIngredients()
{
    this->_ingredientlock.lock();
}

void Kitchen::unlockIngredients()
{
    this->_ingredientlock.unlock();
}

bool Kitchen::getPizzaStock(APizza *pizza)
{
    return (this->_ingredients->getStock(pizza));
}

void Kitchen::removeIngredients(APizza *pizza)
{
    return (this->_ingredients->removeInStock(pizza));
}

// APizza *Kitchen::getPizza(void)
// {
//     APizza *ret = NULL;
//     if (this->_orders.size() == 0) {
//         return (NULL);
//     }
//     else
//     {
//         ret = this->_orders.top();
//         this->_orders.pop();
//     }
//     return (ret);
// }

void Kitchen::addPizza(APizza *pizza)
{
    _orders.push(pizza);
    std::cout << "Pizza Taken" << std::endl;
}
