#include "train.hpp"

#include <math.h>

namespace loopline
{
    Train::Train()
        : railPosition(0.f), speed(0.f), acceleration(0.f)
    {
        if (!slimeTexture.loadFromFile("slime_monster_24x24/slime_monster_spritesheet.png"))
        {
            // error...
        }
        sprite.setTexture(slimeTexture);
        sprite.setTextureRect(sf::IntRect{0, 48, 24, 24});

        accel = std::make_shared<LambdaCommand>([this]() { this->acceleration = 50.f;});
        deaccel = std::make_shared<LambdaCommand>([this]() { this->acceleration -= 50.f;});
        noaccel = std::make_shared<LambdaCommand>([this]() { this->acceleration = -25.f;});
    }

    Train::~Train()
    {

    }

    void Train::setWorldposition(sf::Vector2f const &worldPos)
    {
        sprite.setPosition(worldPos);
    }

    void Train::update(sf::Time const &deltaTime)
    {
    }

    void Train::fixedUpdate(sf::Time const &deltaTime)
    {
        railPosition += speed * deltaTime.asSeconds();

        speed += acceleration * deltaTime.asSeconds();

        if (speed < 0) speed = 0.0f;
        else if (speed >= maxSpeed) speed = maxSpeed;
    } 
    void Train::draw(sf::RenderWindow &window) const
    {
        window.draw(this->sprite);
    }

} // namespace loopline