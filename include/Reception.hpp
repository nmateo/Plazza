/*
** EPITECH PROJECT, 2020
** CCP_plazza_2019
** File description:
** TODO: add description
*/


#ifndef CCP_PLAZZA_2019_RECEPTION_HPP
#define CCP_PLAZZA_2019_RECEPTION_HPP

#include "Plazza.hpp"
#include "APizza.hpp"
#include <sys/select.h>

typedef struct kitchen_control_s {
    int socket;
    bool available;
    int currCooks;
    short id;
} k_ctrl_t;

class Reception
{
    public:
        Reception(double, size_t, size_t);
        ~Reception();
        int shell();
     
    private:
        int parseCommands();
        void openKitchen();
        int initMasterSocket();
        int acceptNewCreatedKitchen();
        int dispatchOrder(PizzaType, PizzaSize);
        int writeOrder(int, PizzaType, PizzaSize);
        void closeKitchen(std::unique_ptr<k_ctrl_t> &);
        int getActiveInputs();
        int handleInputs();
        int handleWorkers();
        int run();
        void handleKitchenRet(std::unique_ptr<k_ctrl_t> &);
        int isAvailableKitchens();
        void printStatus(char **, std::unique_ptr<k_ctrl_t> &);

    protected:
        int _sockMaster;
        fd_set _readfds;
        std::vector<std::unique_ptr<k_ctrl_t>> _kitchens;
        std::list<APizza *> _orders;
        double _multiplier; //Cooking time multiplier.
        size_t _cooks; //Number of cook(s) per kitchen.
        size_t _restock; //Time in ms to restock ingredients.
};


#endif //CCP_PLAZZA_2019_RECEPTION_HPP
