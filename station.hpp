#ifndef __LOOPLINE_STATION_HPP__
#define __LOOPLINE_STATION_HPP__

#include <SFML/Graphics.hpp>

namespace loopline
{
    class Station
    {
    private:
        sf::Vector2f position;
        float radius;
        sf::Sprite sprite;
        sf::Texture slimeTexture;

    public:
        Station();
        Station(sf::Vector2f const& pos, float r);
        ~Station();

        virtual void draw(sf::RenderWindow &window) const;
    };
} // namespace loopline

#endif