#include "station.hpp"

namespace loopline
{
    Station::Station(){}
    Station::Station(sf::Vector2f const& pos, float r, int i)
        : position(pos), radius(r), id(i)
    {
    }

    Station::~Station()
    {
    }

} // namespace loopline
