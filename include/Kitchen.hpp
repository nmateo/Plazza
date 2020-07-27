/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** TODO: add description
*/


#ifndef CCP_PLAZZA_2019_KITCHEN_HPP
#define CCP_PLAZZA_2019_KITCHEN_HPP

#include "Plazza.hpp"
#include "APizza.hpp"
#include "Stock.hpp"
#include <list>

class Kitchen
{
    public:
        Kitchen(double, size_t, size_t, int);
        ~Kitchen();
        APizza *MakeOrder(PizzaType, PizzaSize);
        void status();
        void addPizza(APizza *pizza);
        bool isFull();
        void run();
        void restock();

        void lockPizzas();
        void unlockPizzas();

        void lockIngredients();
        void unlockIngredients();

        APizza *getPizza();
        bool getPizzaStock(APizza *);
        void removeIngredients(APizza *);
        int initControlSocket();
        void processMsg();
        void isAvailable();
        void addOrder(char **toks);
        void setFinPizza(APizza *);
        Stock *_ingredients;

    protected:
        std::stack<APizza *> \
            _orders;
        std::stack<APizza *> \
            _finOrders;
        std::mutex p_mutex;
        std::list<std::thread> _cooks;
        std::stack<APizza *> _finishedPizza;      
        std::vector<bool> _cookStatus;  
        int _socket;
        std::mutex _pizzalock;
        std::mutex _ingredientlock;
        double _multiplier; //Cooking time multiplier.
        size_t _ncooks; //Number of cook(s) per kitchen.
        size_t _restock; //Time in ms to restock ingredients.
        size_t _ordersNow;
        int _id;
};

#endif
