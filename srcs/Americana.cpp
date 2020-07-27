#include "../include/Plazza.hpp"
#include "../include/Americana.hpp"

PizzaAmericana::PizzaAmericana(PizzaSize size)
{
    this->_size = size;
    this->_type = PizzaType::Americana;
    this->_cookTime = 2;
    this->_ingredients = 0;
    this->_ingredients |= Doe;
    this->_ingredients |= Tomato;
    this->_ingredients |= Gruyere;
    this->_ingredients |= Steak;
}

PizzaAmericana::~PizzaAmericana()
{

}
