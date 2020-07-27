#include "../include/Plazza.hpp"
#include "../include/Reception.hpp"
#include "../include/Kitchen.hpp"
#include "../include/Regina.hpp"
#include "../include/Fantasia.hpp"
#include "../include/Americana.hpp"
#include "../include/Margarita.hpp"
#include <string>
#include <stdio.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int isWhitespaces(char *str)
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ')
            return (1);
    }
    return (0);
}

char **sepTok(char *buffer, char *seps)
{
    char **tok = (char **)malloc(sizeof(char *) * 16);
    char *ptr;
    int i = 0;

    if (tok == NULL)
        return (NULL);
    if((ptr = strtok(buffer, seps)) == NULL)
        return (NULL);
    tok[i] = ptr;
    while ((ptr = strtok(NULL, seps)) != NULL) {
        i++;
        if (isWhitespaces(ptr) == 0) {
            i--;
            continue;
        }
        tok[i] = ptr;
        if (i >= 14)
            break;
    }
    tok[++i] = NULL;
    return (tok);
}

int sendMsgSocket(int code, char *str, int socket)
{
    if (str != NULL)
        dprintf(socket, "%d;%s\r\n", code, str);
    else
        dprintf(socket, "%d\r\n", code);
    return (0);
}

char *readMsgSocket(int socket)
{
    size_t idx = 0;
    char *buff = (char *)malloc(sizeof(char) * 2);
    char *tmp = NULL;

    if (buff == NULL)
        return (NULL);
    while (idx < 8192 && read(socket, &buff[idx], 1)) {
        if (idx > 0 && buff[idx - 1] == '\r' && buff[idx] == '\n')
            break;
        idx++;
        buff[idx] = '\0';
        if ((tmp = strdup(buff)) == NULL)
            return (NULL);
        free(buff);
        if ((buff = (char *)malloc(sizeof(char) * (idx + 2))) == NULL)
            return (NULL);
        strcpy(buff, tmp);
    }
    buff[idx - 1] = '\0';
    return (buff);
}

size_t getPizzaNbr(std::string str)
{
    size_t i = 0;
    std::string nbr;
    errno = 0;
    char *endptr;
    size_t ret;

    while (str[i]) {
        if (str[i] == 'x' && isdigit(str[i + 1]))
        {
            nbr = &str[i + 1];
            ret = strtol(nbr.c_str(), &endptr, 0);
            switch (errno)
            { case 0: return (ret); default: return (0); }
        }
        i++;
    }
    return (0);
}

PizzaType getPizzaType(std::string str)
{
    if (str.find("Regina ") !=  std::string::npos)
        return (PizzaType::Regina);
    if (str.find("Margarita ") !=  std::string::npos)
        return (PizzaType::Margarita);
    if (str.find("Fantasia ") !=  std::string::npos)
        return (PizzaType::Fantasia);
    if (str.find("Americana ") !=  std::string::npos)
        return (PizzaType::Americana);
    return (PizzaType(0));
}

std::string getPizzaTypeStr(std::string str)
{
    if (str.find("Regina ") !=  std::string::npos)
        return ("Regina");
    if (str.find("Margarita ") !=  std::string::npos)
        return ("Margarita");
    if (str.find("Fantasia ") !=  std::string::npos)
        return ("Fantasia");
    if (str.find("Americana ") !=  std::string::npos)
        return ("Americana");
    return ("Type not found.");
}

std::string getPizzaSizeStr(std::string str)
{
    if (str.find("XXL ") != std::string::npos)
        return ("XXL");
    if (str.find("XL ") != std::string::npos)
        return ("XL");
    if (str.find("S ") != std::string::npos)
        return ("S");
    if (str.find("M ") != std::string::npos)
        return ("M");
    if (str.find("L ") != std::string::npos)
        return ("L");
    return ("Size not found.");
}

PizzaType getPizzaTypeByInt(std::string str)
{
    if (str.find("1") !=  std::string::npos)
        return (PizzaType::Regina);
    if (str.find("2") !=  std::string::npos)
        return (PizzaType::Margarita);
    if (str.find("4") !=  std::string::npos)
        return (PizzaType::Americana);
    if (str.find("8") !=  std::string::npos)
        return (PizzaType::Fantasia);
    return (PizzaType(0));
}

PizzaSize getPizzaSize(std::string str)
{
    if (str.find("XXL ") != std::string::npos)
        return (PizzaSize::XXL);
    if (str.find("XL ") != std::string::npos)
        return (PizzaSize::XL);
    if (str.find("S ") != std::string::npos)
        return (PizzaSize::S);
    if (str.find("M ") != std::string::npos)
        return (PizzaSize::M);
    if (str.find("L ") != std::string::npos)
        return (PizzaSize::L);
    return (PizzaSize(0));
}

PizzaSize getPizzaSizeByInt(std::string str)
{
    if (str.find("16") != std::string::npos)
        return (PizzaSize::XXL);
    if (str.find("8") != std::string::npos)
        return (PizzaSize::XL);
    if (str.find("4") != std::string::npos)
        return (PizzaSize::S);
    if (str.find("2") != std::string::npos)
        return (PizzaSize::M);
    if (str.find("1") != std::string::npos)
        return (PizzaSize::L);
    return (PizzaSize(0));
}

std::string getPizzaTypeByIntToStr(std::string str)
{
    if (str.find("1") !=  std::string::npos)
        return ("Regina");
    if (str.find("2") !=  std::string::npos)
        return ("Margarita");
    if (str.find("4") !=  std::string::npos)
        return ("Americana");
    if (str.find("8") !=  std::string::npos)
        return ("Fanatasia");
    return ("");
}

std::string getPizzaSizeByIntToStr(std::string str)
{
    if (str.find("16") != std::string::npos)
        return ("XXL");
    if (str.find("8") != std::string::npos)
        return ("XL");
    if (str.find("4") != std::string::npos)
        return ("S");
    if (str.find("2") != std::string::npos)
        return ("M");
    if (str.find("1") != std::string::npos)
        return ("L");
    return ("");
}

std::string getPizzaSizeByPizza(APizza *pizza)
{
    switch (pizza->getSize())
    {
        case PizzaSize::XXL:
            return ("XXL");
        case PizzaSize::XL:
            return ("XL");
        case PizzaSize::S:
            return ("S");
        case PizzaSize::M:
            return ("M");
        case PizzaSize::L:
            return ("L");
        default:
            return("");
    }
}

std::string getPizzaTypeByPizza(APizza *pizza)
{
    switch (pizza->getType())
    {
        case PizzaType::Regina:
            return ("Regina");
        case PizzaType::Margarita:
            return ("Margarita");
        case PizzaType::Fantasia:
            return ("Fantasia");
        case PizzaType::Americana:
            return ("Americana");
        default:
            return("");
    }
}

APizza *makeOrder(PizzaType type, PizzaSize size)
{
    APizza *pizza;
    switch (type)
    {
        case Regina:
            pizza = new PizzaRegina(size);
            return (pizza);
        case Margarita:
            pizza = new PizzaMargarita(size);
            return (pizza);
        case Fantasia:
            pizza = new PizzaFantasia(size);
            return (pizza);
        case Americana:
            pizza = new PizzaAmericana(size);
            return (pizza);
        default:
            return NULL;
    }
    return (NULL);
}