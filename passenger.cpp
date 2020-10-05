#include "passenger.hpp"

#include <algorithm>

namespace loopline
{
    Passenger::Passenger(){}
    Passenger::Passenger(int enterS, int exitS, bool boardedT, bool killP)
        : enterStation(enterS), exitStation(exitS), boardedTrain(boardedT), killPassenger(killP)
    {
        waitingTime.restart();
    }

    Passenger::~Passenger()
    {
    }

    void Passenger::update(sf::Time const &deltaTime)
    {

    }

    void Passenger::fixedUpdate(sf::Time const &deltaTime)
    {
        if(!boardedTrain && waitingTime.getElapsedTime() > maxWaitingTime) killPassenger = true;
    }

    int Passenger::payTicket()
    {
        sf::Time waited = waitingTime.getElapsedTime();

        if(waited < minWaitingTime) return gold;
        
        return std::max(0.f, (1.f - (waited - minWaitingTime) / (maxWaitingTime - minWaitingTime)) * gold);
    }

} // namespace loopline

