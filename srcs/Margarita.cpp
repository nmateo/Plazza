#include "../include/Plazza.hpp"
#include "../include/Margarita.hpp"

PizzaMargarita::PizzaMargarita(PizzaSize size)
{
    this->_size = size;
    this->_type = PizzaType::Margarita;
    this->_cookTime = 1;
    this->_ingredients = 0;
    this->_ingredients |= Doe;
    this->_ingredients |= Tomato;
    this->_ingredients |= Gruyere;
}

PizzaMargarita::~PizzaMargarita()
{

}
