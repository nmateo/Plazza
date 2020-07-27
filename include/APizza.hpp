/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** TODO: add description
*/


#ifndef CCP_PLAZZA_2019_APIZZA_HPP
#define CCP_PLAZZA_2019_APIZZA_HPP

#include "Plazza.hpp"

class APizza
{
    public:
        virtual ~APizza() = 0;
        PizzaType getType()const; 
        PizzaSize getSize()const;
        int getCookTime() const;
        size_t getIngredients();

    protected:
        PizzaSize _size;
        PizzaType _type;
        size_t _ingredients;
        int _cookTime;
};

#endif //CCP_PLAZZA_2019_APIZZA_HPP
