#include "train.hpp"

#include <math.h>

namespace loopline
{
    Train::Train()
        : railPosition(0.f), speed(00.f), acceleration(0.f), length(48.f)
    {
        accel = std::make_shared<AccelCommand>(this, 50.f);
        deaccel = std::make_shared<AccelCommand>(this, -50.f);
        noaccel = std::make_shared<AccelCommand>(this, -25.f);
    }

    Train::~Train()
    {

    }

    Train::Train(Train const &train)
        : railPosition(train.railPosition), speed(train.speed), acceleration(train.acceleration), length(train.length)
    {
        accel = std::make_shared<AccelCommand>(this, 50.f);
        deaccel = std::make_shared<AccelCommand>(this, -50.f);
        noaccel = std::make_shared<AccelCommand>(this, -25.f);
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

        if (speed < 0.0001f) state = STOPPED;
        else state = DRIVING;
    } 

    void Train::drawUI(sf::RenderWindow &window, sf::Font const &font)
    {
        std::string capacity = std::to_string(passengers) + " / " + std::to_string(maxCapacity) + " Passengers";

        sf::View const view = window.getView();

        sf::Text uiText(capacity, font);
        uiText.setFillColor(sf::Color::Black);
        uiText.setOrigin(uiText.findCharacterPos(capacity.size()-1) + 0.5f * sf::Vector2f{static_cast<float>(uiText.getCharacterSize()), static_cast<float>(uiText.getCharacterSize())});

        sf::Vector2f downRight = view.getCenter() + 0.5f * sf::Vector2f{view.getSize().x, view.getSize().y};
        uiText.setPosition(downRight - sf::Vector2f{30.f, 30.f});

        window.draw(uiText);
    }

    Train::AccelCommand::AccelCommand(Train *train, float accel)
        : train(train), accel(accel)
    {
    }

    Train::AccelCommand::~AccelCommand()
    {
    }

    void Train::AccelCommand::execute()
    {
        if(train != NULL) train->acceleration = accel;
    }

    void Train::AccelCommand::undo()
    {
    }

} // namespace loopline