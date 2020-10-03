#ifndef __LOOPLINE_PASSENGER_HPP__
#define __LOOPLINE_PASSENGER_HPP__

namespace loopline
{
    class Passenger
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
    };
} // namespace loopline

#endif