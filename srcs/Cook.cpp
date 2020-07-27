#include "../include/Cook.hpp"
#include <unistd.h>
#include <thread>
#include <chrono>
#include <csignal>

Cook::Cook(Kitchen *kitchen, double multiplier, int cNumber, int kNumber) :
    _kitchen(std::ref(kitchen)), _isCooking(false), _id(cNumber), _kId(kNumber)
{
    this->_currPizza = NULL;
    this->_multiplier = multiplier;
}

Cook::~Cook()
{

}

void Cook::cookPizza(APizza *pizza)
{
    (void)(pizza);
}

bool Cook::canCook()
{
    return (!this->_isCooking);
}

bool Cook::isCooking()
{
    return (this->_isCooking);
}

void Cook::getOrder(std::stack<APizza *> &orders, std::mutex &p_mutex)
{
    this->_currPizza = orders.top();
    orders.pop();
}

void Cook::Run(std::stack<APizza *> &orders, std::stack<APizza *> &finOrders, std::mutex &p_mutex, std::vector<bool> &cookStatus)
{

    while (1)
    {
        if (orders.size() > 0)
        {
            std::lock_guard<std::mutex> guard(p_mutex);
            if (orders.size() > 0)
            {
                getOrder(orders, p_mutex);
                this->_isCooking = true;
            }
        }
        if (this->_currPizza != NULL)
        {
            cookStatus[_id] = true;
            _kitchen->_ingredients->removeInStock(_currPizza);
            int cookTime = (int)_currPizza->getCookTime() * this->_multiplier;
            std::cout << "[Cook " << _id << " from kitchen n°" << _kId << "] " \
            << "Starting to cook a(n) " << getPizzaTypeByPizza(_currPizza) \
            << " of size " << getPizzaSizeByPizza(_currPizza) << ", the cooking time is " << _currPizza->getCookTime() * _multiplier << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(cookTime));
            std::cout << "[Cook " << _id << " from kitchen n°" << _kId << "] " \
            << "Just finished cooking a(n) " << getPizzaTypeByPizza(_currPizza) \
            << " of size " << getPizzaSizeByPizza(_currPizza) << "." << std::endl;
            fflush(stdout);
            finOrders.push(_currPizza);
            this->_currPizza = NULL;
            this->_isCooking = false;
            cookStatus[_id] = false;
        }
    }
}
