#include "train.hpp"

#include <math.h>

namespace loopline
{
    Train::Train()
        : position({400.f, 300.f}), speed(0.f), acceleration(0.f), accel(std::make_shared<LambdaCommand>([this]() { this->acceleration = 0.20f; })), 
                deaccel(std::make_shared<LambdaCommand>([this]() { this->acceleration -= 0.20f; })), noaccel(std::make_shared<LambdaCommand>([this]() { this->acceleration = 0.f; }))
    {
        if (!slimeTexture.loadFromFile("slime_monster_24x24/slime_monster_spritesheet.png"))
        {
            // error...
        }
        sprite.setTexture(slimeTexture);
        sprite.setTextureRect(sf::IntRect{0, 48, 24, 24});
    }

    Train::~Train()
    {

    }

    void Train::update(sf::Time const &deltaTime)
    {
        sprite.setPosition(position);
    }

    void Train::fixedUpdate(sf::Time const &deltaTime)
    {
        // TODO: Schienen müssen das positionsupdate übernehmen?
        position += sf::Vector2f{speed, 0.f};
        position.x = fmod(position.x, 800.f);

        speed += acceleration;
    }
    
    void Train::draw(sf::RenderWindow &window)
    {
        window.draw(this->sprite);
    }

} // namespace loopline