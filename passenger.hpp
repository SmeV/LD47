#ifndef __LOOPLINE_PASSENGER_HPP__
#define __LOOPLINE_PASSENGER_HPP__

#include "updatable.hpp"

namespace loopline
{
    class Passenger : public Updatable
    {
    private:

    public:
        Passenger();
        Passenger(int enterS, int exitS, bool boardedT = false, bool killPassenger = false);
        ~Passenger();

        int enterStation;
        int exitStation;
        bool boardedTrain;
        bool killPassenger;

        sf::Clock waitingTime;
        sf::Time maxWaitingTime = sf::seconds(60.0f);
        sf::Time minWaitingTime = sf::seconds(30.0f);
        int gold = 10;

        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);

        int payTicket();
    };
} // namespace loopline

#endif