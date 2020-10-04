#include "train.hpp"

#include <math.h>

namespace loopline
{
    Train::Train(float railPosition, float speed, float acceleration, float length)
        : railPosition(railPosition), speed(speed), acceleration(acceleration), length(length)
    {
        accel = std::make_shared<AccelCommand>(this, 100.f);
        deaccel = std::make_shared<AccelCommand>(this, -200.f);
        noaccel = std::make_shared<AccelCommand>(this, -125.f);
    }

    Train::~Train()
    {

    }

    Train::Train(Train const &train)
        : railPosition(train.railPosition), speed(train.speed), acceleration(train.acceleration), length(train.length)
    {
        accel = std::make_shared<AccelCommand>(this, 100.f);
        deaccel = std::make_shared<AccelCommand>(this, -200.f);
        noaccel = std::make_shared<AccelCommand>(this, -125.f);
    }

    void Train::addWagon(TrainWagon const &wagon)
    {
        wagons.push_back(wagon);

        maxCapacity += 20;
    }

    void Train::update(sf::Time const &deltaTime)
    {
        for(auto &wagon : wagons)
        {
            wagon.update(deltaTime);
        }
    }

    void Train::fixedUpdate(sf::Time const &deltaTime)
    {
        railPosition += speed * deltaTime.asSeconds();

        speed += acceleration * deltaTime.asSeconds();

        if (speed < 0) speed = 0.0f;
        else if (speed >= maxSpeed) speed = maxSpeed;

        if (speed < 0.0001f) state = STOPPED;
        else state = DRIVING;

        for(auto &wagon : wagons)
        {
            wagon.fixedUpdate(deltaTime);
        }
    } 

    void Train::draw(sf::RenderWindow &window) const
    {
        for(auto &wagon : wagons)
        {
            wagon.draw(window);
        }
        Drawable::draw(window);
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