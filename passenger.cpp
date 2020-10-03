#include "passenger.hpp"

namespace loopline
{
    Passenger::Passenger(){}
    Passenger::Passenger(int enterS, int exitS, bool boardedT, bool killP)
        : enterStation(enterS), exitStation(exitS), boardedTrain(boardedT), killPassenger(killP)
    {
    }

    Passenger::~Passenger()
    {
    }

} // namespace loopline

