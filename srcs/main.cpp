#include "../include/Plazza.hpp"
#include "../include/Reception.hpp"
#include <string>
#include <cstdlib>

static bool is_digits(const std::string &str)
{
    return str.find_first_not_of("0123456789.") == std::string::npos;
}

static int usage(int ret)
{
    std::cout << "./plazza {cooking time multiplier} {number of cooks per kitchen} {time in ms to restock ingredients}" << std::endl;
    return ret;
}

int main(int ac, char **av)
{
    errno = 0;
    char *endptr;
    double multiplier;
    size_t cooks;
    size_t restock;

    if (ac != 4)
        return usage(84);
    if (is_digits((std::string)av[1]) == false || \
            is_digits((std::string)av[2]) == false || \
            is_digits((std::string)av[3]) == false)
        return usage(84);
    multiplier = strtod(av[1], &endptr);
    switch (errno) {
        case 0: break;
        default: return usage(84);
    }
    cooks = strtol(av[2], &endptr, 0);
    switch (errno) {
        case 0: break;
        default: return usage(84);
    }
    restock = strtol(av[3], &endptr, 0);
    switch (errno) {
        case 0: break;
        default: return usage(84);
    }
    Reception *plazza = new Reception(multiplier, cooks, restock);
    (void)(plazza);
    return 0;
}
