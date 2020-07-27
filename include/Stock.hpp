/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** TODO: add description
*/


#ifndef CCP_PLAZZA_2019_STOCK_HPP
#define CCP_PLAZZA_2019_STOCK_HPP

#include <string>
#include <bits/stdc++.h>
#include "APizza.hpp"

class Stock
{
    public:
        Stock();
        ~Stock();
        bool getStock(APizza *);
        void addStock();
        void removeInStock(APizza *);
        char *getStock();
    private:
        size_t _doe;
        size_t _gruyere;
        size_t _tomato;
        size_t _ham;
        size_t _mushroom;
        size_t _steak;
        size_t _eggplant;
        size_t _goatcheese;
        size_t _chieflove;
};

#endif
