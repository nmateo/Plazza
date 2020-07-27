#include "../include/Plazza.hpp"
#include "../include/APizza.hpp"

APizza::~APizza()
{

}

PizzaSize APizza::getSize(void) const
{
    return (this->_size);
}

PizzaType APizza::getType(void) const
{
    return (this->_type);
}

int APizza::getCookTime(void) const
{
    return (this->_cookTime);
}

size_t APizza::getIngredients(void)
{
    return (this->_ingredients);
}
