#include "../include/Plazza.hpp"
#include "../include/Regina.hpp"

PizzaRegina::PizzaRegina(PizzaSize size)
{
    this->_size = size;
    this->_type = PizzaType::Regina;
    this->_cookTime = 2;
    this->_ingredients = 0;
    this->_ingredients |= Doe;
    this->_ingredients |= Tomato;
    this->_ingredients |= Gruyere;
    this->_ingredients |= Ham;
    this->_ingredients |= Mushroom;
}

PizzaRegina::~PizzaRegina()
{

}
