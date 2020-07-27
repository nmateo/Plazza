#include "../include/Plazza.hpp"
#include "../include/Stock.hpp"

Stock::Stock() :
    _doe(5), _gruyere(5), _tomato(5), _ham(5), _mushroom(5), _steak(5), _eggplant(5), _goatcheese(5), _chieflove(5)
{
    
}

Stock::~Stock()
{

}

bool Stock::getStock(APizza *pizz)
{
    size_t ingredients = pizz->getIngredients();

    if (ingredients & Ingredient::Doe && this->_doe <= 0)
        return (false);
    if (ingredients & Ingredient::Gruyere && this->_gruyere <= 0)
        return (false);
    if (ingredients & Ingredient::Tomato && this->_tomato <= 0)
        return (false);
     if (ingredients & Ingredient::Ham && this->_ham <= 0)
        return (false);
    if (ingredients & Ingredient::Mushroom && this->_mushroom <= 0)
        return (false);
    if (ingredients & Ingredient::Steak && this->_steak <= 0)
        return (false);
    if (ingredients & Ingredient::Eggplant && this->_eggplant <= 0)
        return (false);
    if (ingredients & Ingredient::GoatCheese && this->_goatcheese <= 0)
        return (false);
    if (ingredients & Ingredient::ChiefLove && this->_chieflove <= 0)
        return (false);
    return (true);
}

void Stock::removeInStock(APizza *pizz)
{
    size_t ingredients = pizz->getIngredients();

    if (ingredients & Ingredient::Doe && this->_doe > 0)
        this->_doe--;
    if (ingredients & Ingredient::Gruyere && this->_gruyere > 0)
        this->_gruyere--;
    if (ingredients & Ingredient::Tomato && this->_tomato > 0)
        this->_tomato--;
    if (ingredients & Ingredient::Ham && this->_ham > 0)
        this->_ham--;
    if (ingredients & Ingredient::Mushroom && this->_mushroom > 0)
        this->_mushroom--;
    if (ingredients & Ingredient::Steak && this->_steak > 0)
        this->_steak--;
    if (ingredients & Ingredient::Eggplant && this->_eggplant > 0)
        this->_eggplant--;
    if (ingredients & Ingredient::GoatCheese && this->_goatcheese > 0)
        this->_goatcheese--;
    if (ingredients & Ingredient::ChiefLove && this->_chieflove > 0)
        this->_chieflove--;
}

void Stock::addStock()
{
    _doe += 1;
    _gruyere += 1;
    _tomato += 1;
    _ham += 1;
    _mushroom += 1;
    _steak += 1;
    _eggplant += 1;
    _goatcheese += 1;
    _chieflove += 1;
}

char *Stock::getStock()
{
    char *str = "";
    asprintf(&str, "%ld;%ld;%ld;%ld;%ld;%ld;%ld;%ld;%ld", _doe, _gruyere, _tomato, _ham, _mushroom, _steak, _eggplant, _goatcheese, _chieflove);
    return (str);
}
