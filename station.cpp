#include "station.hpp"

namespace loopline
{
    Station::Station(){}
    Station::Station(sf::Vector2f const& pos, float r)
        : position(pos), radius(r)
    {
        if (!slimeTexture.loadFromFile("slime_monster_24x24/slime_monster_spritesheet.png"))
        {
            // error...
        }
        sprite.setTexture(slimeTexture);
        sprite.setTextureRect(sf::IntRect{24, 48, 72, 24});
        sprite.setOrigin(60.f, 12.f);
    }

    Station::~Station()
    {
    }

    void Station::draw(sf::RenderWindow &window) const
    {
        window.draw(this->sprite);
    }

} // namespace loopline
