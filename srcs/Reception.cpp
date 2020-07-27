#include "../include/Plazza.hpp"
#include "../include/Reception.hpp"
#include "../include/Kitchen.hpp"
#include "../include/Regina.hpp"
#include "../include/Fantasia.hpp"
#include "../include/Americana.hpp"
#include "../include/Margarita.hpp"
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <utility>

using uKCtrl = std::unique_ptr<k_ctrl_t>;

Reception::Reception(double multiplier, size_t cooks, size_t restock) :
    _multiplier(multiplier), _cooks(cooks), _restock(restock) 
{
    std::cout << "Reception open !" << std::endl << "Cooking time multiplier is " << _multiplier <<
        ", there is " << _cooks  << " cooks per kitchen and kitchens restock every " <<
        _restock << " milliseconds." << std::endl;
    this->initMasterSocket();
    this->run();
}

Reception::~Reception()
{

}

int Reception::initMasterSocket()
{
    int op = 1;
    struct in_addr inaddr = {htonl(INADDR_ANY)};
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    if ((this->_sockMaster = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket()");
        exit(84);
    }
    if (setsockopt(this->_sockMaster, SOL_SOCKET, (SO_REUSEADDR | SO_REUSEPORT), (char *)&op, sizeof(op)) < 0) {
        perror("setsockopt()");
        exit(84);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000);
    addr.sin_addr = inaddr;
    if (bind(this->_sockMaster, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind()");
        exit(84);
    }
    if (listen(this->_sockMaster, MAX_REQ) <0)
    {
        perror("listen()");
        exit(84);
    }
    return (0);
}

int Reception::acceptNewCreatedKitchen()
{
    int tmp;
    struct sockaddr_in saddr_client;
    int clen = sizeof(struct sockaddr_in);
    memset(&saddr_client, 0, sizeof(struct sockaddr_in));
    if (FD_ISSET(this->_sockMaster, &this->_readfds))
    {
        if ((tmp = accept(this->_sockMaster, \
                        (struct sockaddr *)&saddr_client, \
                        (socklen_t *)&clen)) < 0) {
            perror("accept()");
            exit(84);
        }
        std::unique_ptr<k_ctrl_t> curr = std::make_unique<k_ctrl_t>();
        curr->available = false;
        curr->socket = tmp;
        curr->id = _kitchens.size();
        curr->currCooks = 0;
        this->_kitchens.push_back(std::move(curr));
        // printf("pushed kitchen\n");
    }
    return (0);
}

int Reception::shell()
{
    std::string command;
    std::list<std::string> orders;

    getline (std::cin, command);
    fflush(stdin);
    if (!command.compare("exit") || std::cin.eof() || !command.compare("status"))
    {
        if  (!command.compare("exit") || std::cin.eof())
        {
            for (int i = 0; i != _kitchens.size(); i++)
            {
                sendMsgSocket(CLOSE_KITCHEN, NULL, _kitchens[i]->socket);
            }
            exit(0);
        } else
        {
            for (int i = 0; i != _kitchens.size(); i++)
            {
                sendMsgSocket(STATUS_KITCHEN, NULL, _kitchens[i]->socket);
            }
        }
    }
    else
    {
        orders = splitStr(command, ';');
    }
    if (!orders.empty())
    {
        for (const std::string & order : orders)
        {
            size_t nPizza = getPizzaNbr(order);
            PizzaType tPizza = getPizzaType(order);
            PizzaSize sPizza = getPizzaSize(order);
            if (nPizza == 0 || tPizza == (PizzaType)0 || sPizza == (PizzaSize)0)
                continue;
            std::cout << "\033[1;31m[Reception] Your " << nPizza << " " << getPizzaTypeStr(order) \
            << " of size " << getPizzaSizeStr(order) << \
            " are being handled by our chiefs.\033[0m" << std::endl;
            for (size_t i = 0; i != nPizza; i++)
            {
                APizza *pizza;
                pizza = makeOrder(tPizza, sPizza);
                if (pizza != NULL)
                    this->_orders.push_back(pizza);
            }
        }
    }
    return 0;    
}

int Reception::getActiveInputs()
{
    int max_fd = 0;
    int sockfd;
    errno = 0;
    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    (0 > this->_sockMaster) ? max_fd = 0: max_fd = this->_sockMaster;
    FD_ZERO(&this->_readfds);
    FD_SET(0, &this->_readfds);
    FD_SET(this->_sockMaster, &this->_readfds);
    for (int i = 0; i < (int)_kitchens.size(); i++)
    {
        sockfd = _kitchens[i]->socket;
        FD_SET(sockfd, &this->_readfds);
        (sockfd > max_fd ? max_fd = sockfd:0); 
    }
    if (select(max_fd + 1, &this->_readfds, NULL, NULL, &tv) < 0 && \
            (errno != EINTR))
    {
        perror("select()");
        exit(84);
    }
    return (0);
}

int Reception::run()
{
    while (1)
    {
        this->getActiveInputs();
        this->acceptNewCreatedKitchen();
        this->handleInputs();
        this->handleWorkers();
    }
    return 0;
}

int Reception::handleInputs()
{
    if (FD_ISSET(0, &this->_readfds))
        this->shell();
    for (int i = 0; i < (int)_kitchens.size(); i++)
    {
        if (FD_ISSET(_kitchens[i]->socket, &this->_readfds))
        {
            handleKitchenRet(std::ref<std::unique_ptr<k_ctrl_t>>(_kitchens[i]));
        }
    }
    return (0);
}

int Reception::handleWorkers()
{
    if (isAvailableKitchens() == -1 && _orders.size() > 0)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            Kitchen kitchen(_multiplier, _cooks, _restock, _kitchens.size());
            kitchen.run();
            exit(0);
        }
        usleep(200000);
    }
    else if (_orders.size() > 0 && isAvailableKitchens() == 0)
    {
        for (int i = 0; i < (int)_kitchens.size(); i++)
        {
            if (_orders.size() == 0)
                break;
            APizza *newPizza;
            if (_kitchens[i]->available == true)
            {
                newPizza = _orders.front();
                _kitchens[i]->currCooks++;
                writeOrder(_kitchens[i]->socket, newPizza->getType(), newPizza->getSize());
                _orders.pop_front();
                usleep(50000);
            }
        }

    }
    return (0);
}

void Reception::handleKitchenRet(std::unique_ptr<k_ctrl_t> &kitch)
{
    char *msg = readMsgSocket(kitch->socket);
    char **toks = sepTok(msg, (char *)(const char *)";\r\n");

    if (toks == NULL || toks[0] == NULL)
        return;
    switch (atoi(toks[0]))
    {
        case FIN_ORDER:
            std::cout << "\033[1;31m[Reception] Pizza " << getPizzaTypeByIntToStr(toks[1]) << " of size " << getPizzaSizeByIntToStr(toks[2]) << " is served.\033[0m" << std::endl;
            break;
        case AVAILABLE:
            kitch->available = true;
            break;
        case UNAVAILABLE:
            kitch->available = false;
            break;
        case KITCHEN_CLOSED:
            closeKitchen(kitch);
            break;
        case STATUS_KITCHEN:
            printStatus(&toks[1], kitch);
        default:
            break;
    }
}

void Reception::printStatus(char **toks, std::unique_ptr<k_ctrl_t> &kitch)
{
    std::cout << "\033[1;31m[Reception] Kitchen n°" << kitch->id << " status: " << \
    toks[0] << "/" << this->_cooks << " occupied, stock is (" << \ 
    toks[1] << " Doe) (" << toks[2] << " Gruyere) (" << toks[3] << " Tomato) (" << \
    toks[4] << " Ham) (" << toks[5] << " Mushroom) (" << toks[6] << " Steak) (" << \
    toks[7] << " Eggplant) (" << toks[8] << " Goatcheese) (" << toks[9] << " ChiefLove)\033[0m" << \
    std::endl;
}

void Reception::closeKitchen(std::unique_ptr<k_ctrl_t> &kitch)
{
    std::vector<std::unique_ptr<k_ctrl_t>>::iterator it = _kitchens.begin();

    for (; it != _kitchens.end(); it++)
    {
        auto tmp = it;
        if (tmp->get()->socket == kitch->socket)
        {
            _kitchens.erase(it);
        }
    }
}

int Reception::isAvailableKitchens()
{
    int ret = -1;

    for (int i = 0; i < (int)_kitchens.size(); i++)
    {
        if (_kitchens[i]->available == true)
            ret = 0;
    }
    return (ret);
}

int Reception::writeOrder(int socket, PizzaType type, PizzaSize size)
{
    dprintf(socket, "%d;%d;%d\r\n", NEW_ORDER, type, size);
    return (socket);
}
