#include "game.hpp"
#include "helper.hpp"
#include <algorithm>
#include <stdlib.h>
#include <time.h>

namespace loopline
{
    LoopLine::LoopLine()
        : window(sf::VideoMode{800, 600}, "My window"), rails({{10.f, 10.f}, {200.f,400.f},{400.f,100.f},{600.f, 300.f},{790.f, 10.f}, {10.f, 590.f}})
    {
        Station s1(sf::Vector2f(200.f,400.f), 100.f, 0);
        Station s2(sf::Vector2f(600.f,300.f), 100.f, 1);
        Station s3(sf::Vector2f(10.f,590.f), 100.f, 2);

        stations = std::vector<Station> ({s1, s2, s3});

        initializeGame();
    }
    LoopLine::~LoopLine()
    {
    }

    void LoopLine::initializeGame()
    {
        srand (time(NULL));

        textureManager.loadTexture("slime_monster_24x24/slime_monster_spritesheet.png", "slime_spritesheet");
        rails.train.setSprite(textureManager.getTexture("slime_spritesheet"), sf::IntRect{0, 48, 72, 24}, {60.f, 12.f});

        for(auto& station : stations)
        {
            station.setSprite(textureManager.getTexture("slime_spritesheet"), sf::IntRect{24,48,72,24}, {60.f,12.f});
            station.setSpritePosition(station.position);
        }

        inputManager.addEventCommand(rails.train.accel, sf::Keyboard::W);
        inputManager.addEventCommand(rails.train.deaccel, sf::Keyboard::S);
        inputManager.addReleaseEventCommand(rails.train.noaccel, sf::Keyboard::W);
        inputManager.addReleaseEventCommand(rails.train.noaccel, sf::Keyboard::S);

        inputManager.addEventCommand(std::make_shared<loopline::LambdaCommand>([this]() { window.close(); }), sf::Keyboard::Escape);
    }

    void LoopLine::start()
    {
        // run the program as long as the window is open
        while (window.isOpen())
        {
            gameLoop();
        }

    }

    void LoopLine::gameLoop()
    {
        // add the time passed since the last cycle
        elapsed = elapsed + clock.restart();

        // check all the window's events that were triggered since the last iteration of the loop
        // for some events you probably don't want to apply the rule to wait for an update!
        handleEvents();

        // make a single normal update
        update(elapsed);

        // make as many fixed updates as needed for the elapsed time
        while (elapsed > updateTime)
        {
            fixedUpdate(updateTime);

            // don't forget to subtract the updateTime each cycle ;-)
            elapsed -= updateTime;
        }

        // at last: render!
        render();
    }


    void LoopLine::handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                inputManager.eventKey(event.key.code);
                break;
            case sf::Event::KeyReleased:
                inputManager.eventKeyReleased(event.key.code);
                break;
            }
        }
    }

    void LoopLine::update(sf::Time const &deltaTime)
    {
        inputManager.update(deltaTime);

        rails.update(deltaTime);
    }

    void LoopLine::fixedUpdate(sf::Time const &deltaTime)
    {
        inputManager.fixedUpdate(deltaTime);

        rails.fixedUpdate(deltaTime);

        spawnPassengers();

        if(rails.train.speed <= 0.0001)
        {
            //check if within station radius
            for(auto& station : stations)
            {
                if(vectorLength(rails.getWorldPosition(rails.train.railPosition) - station.position) < station.radius)
                {
                    updatePassengers(station.id);
                }
            }
        }
    }

    void LoopLine::render()
    {
        // clear the window with black color
        window.clear(sf::Color::Black);

        for(auto& station : stations)
        {
            station.draw(window);
        }

        rails.draw(window);


        window.display();
    }

    void LoopLine::spawnPassengers()
    {
        if(passengers.size() < maxPassengers && rand() % 10 < 2)
        {
            // TODO: die sollen nicht gleich sein alter
            int in = rand()%(stations.size() - 1);
            int out = rand()%(stations.size() - 1);
            passengers.push_back(Passenger(in, out));
        }
    }

    void LoopLine::updatePassengers(int currentStation)
    {
        for(auto& passenger : passengers)
        {
            if(passenger.enterStation == currentStation)
            {
                passenger.boardedTrain = true;
            }

            if(passenger.exitStation == currentStation)
            {
                passenger.killPassenger = true;

                //TODO increase money!
            }
        }

        passengers.erase(std::remove_if(passengers.begin(), passengers.end(), [] (Passenger p) {return p.killPassenger;}));

    }
} // namespace loopline