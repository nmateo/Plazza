#include "../include/Plazza.hpp"
#include "../include/Fantasia.hpp"

PizzaFantasia::PizzaFantasia(PizzaSize size)
{
    this->_size = size;
    this->_type = PizzaType::Fantasia;
    this->_cookTime = 4;
    this->_ingredients = 0;
    this->_ingredients |= Doe;
    this->_ingredients |= Tomato;
    this->_ingredients |= Gruyere;
    this->_ingredients |= GoatCheese;
    this->_ingredients |= ChiefLove;
}

PizzaFantasia::~PizzaFantasia()
{

}
