#include "Plazza.hpp"
#include "APizza.hpp"
#include "Kitchen.hpp"

class Cook
{
    public:
        Cook(Kitchen *, double, int, int);
        ~Cook();
        void Run(std::stack<APizza *> &, std::stack<APizza *> &, std::mutex &, std::vector<bool> &);
        bool canCook();
        bool isCooking();
        void cookPizza(APizza *);
        void getOrder(std::stack<APizza *> &, std::mutex &);
        void handleSignal(int);

    private:
        Kitchen *_kitchen;
        APizza *_currPizza;
        bool _isCooking;
        double _multiplier;
        int _id;
        int _kId;
        bool _status;
};
