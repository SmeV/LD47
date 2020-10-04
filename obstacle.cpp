#include "obstacle.hpp"


namespace loopline
{

    Obstacle::Obstacle()
    {
        elapsed = sf::Time::Zero;
    }

    void Obstacle::update(sf::Time const &deltaTime)
    {

    }

    void Obstacle::fixedUpdate(sf::Time const &deltaTime)
    {
        elapsed += deltaTime;
        float multiplier = 1.f;

        switch (state)
        {
        case APPROACH:
            if (elapsed >= duration)
            {
                state = BLOCKING;
                elapsed = sf::Time::Zero;
            }
            multiplier = elapsed / fadingTime;
            break;
        case BLOCKING:
            if (elapsed >= duration)
            {
                state = LEAVE;
                elapsed = sf::Time::Zero;
            }
            break;
        case LEAVE:
            if (elapsed >= duration)
            {
                state = APPROACH;
                elapsed = sf::Time::Zero;
            }
            multiplier = 1.f - elapsed / fadingTime;
            break;
        }
        if(type != TRAFFICLIGHT) sprite.setColor(sf::Color{255U, 255U, 255U, static_cast<sf::Uint8>(255.f * multiplier)});
    }

} // namespace loopline