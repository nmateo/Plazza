#ifndef CCP_PLAZZA_2019_HPP
#define CCP_PLAZZA_2019_HPP

#include <cstddef>
#include <vector>
#include <list>
#include <memory>
#include <string>
#include <iostream>

#define KITCHEN_CLOSE_TIME (5)
#define SOCKET_NAME ".masterSocketPlazza"
#define MAX_REQ (100)

#define IS_AVAILABLE 200
#define AVAILABLE 210
#define UNAVAILABLE 220

#define KITCHEN_OPENED 300
#define KITCHEN_CLOSED 310

#define NEW_ORDER 400
#define FIN_ORDER 410

#define CLOSE_KITCHEN 500
#define STATUS_KITCHEN 510

enum PizzaType 
{
    Regina = 1,
    Margarita = 2,
    Americana = 4,
    Fantasia = 8
};

enum PizzaSize
{
    S = 1,
    M = 2,
    L = 4,
    XL = 8,
    XXL = 16
};

enum Ingredient 
{
    Doe = 1,
    Gruyere = 2,
    Tomato = 4,
    Ham = 8,
    Mushroom = 16,
    Steak = 32,
    Eggplant = 64,
    GoatCheese = 128,
    ChiefLove = 256
};

std::list<std::string> splitStr(std::string str, char del);
int sendMsgSocket(int code, char *str, int socket);
char *readMsgSocket(int socket);
char **sepTok(char *buffer, char *seps);

#include "APizza.hpp"

APizza *makeOrder(PizzaType type, PizzaSize size);
PizzaSize getPizzaSize(std::string str);
PizzaType getPizzaType(std::string str);
PizzaSize getPizzaSizeByInt(std::string str);
PizzaType getPizzaTypeByInt(std::string str);
size_t getPizzaNbr(std::string str);

std::string getPizzaTypeStr(std::string str);
std::string getPizzaSizeStr(std::string str);
std::string getPizzaTypeByPizza(APizza *pizza);
std::string getPizzaSizeByPizza(APizza *pizza);
std::string getPizzaTypeByIntToStr(std::string str);
std::string getPizzaSizeByIntToStr(std::string str);


#endif
