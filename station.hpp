#ifndef __LOOPLINE_STATION_HPP__
#define __LOOPLINE_STATION_HPP__

#include "drawable.hpp"

namespace loopline
{
    class Station : public Drawable
    {
    private:

    public:
        Station();
        Station(sf::Vector2f const& pos, float r, int i);
        ~Station();

        sf::Vector2f position;
        float radius;
        int id;

        int maxCapacity = 100;
        int waitingPassengers = 0;

        virtual void drawInfo(sf::RenderWindow &window, sf::Font const &font) const override;
    };
} // namespace loopline

#endif