#include "train.hpp"

#include <math.h>

namespace loopline
{
    Train::Train()
        : railPosition(0.f), speed(0.f), acceleration(0.f), length(48.f)
    {
        accel = std::make_shared<LambdaCommand>([this]() { this->acceleration = 50.f;});
        deaccel = std::make_shared<LambdaCommand>([this]() { this->acceleration -= 50.f;});
        noaccel = std::make_shared<LambdaCommand>([this]() { this->acceleration = -25.f;});
    }

    Train::~Train()
    {

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

} // namespace loopline