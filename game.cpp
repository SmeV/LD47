#include "game.hpp"
#include "helper.hpp"
#include <algorithm>
#include <stdlib.h>
#include <time.h>

#include <iostream>

namespace loopline
{
    LoopLine::LoopLine()
        : window(sf::VideoMode{800, 600}, "Loop Line!"), rails({{10.f, 10.f}, {200.f,400.f},{400.f,100.f},{600.f, 300.f},{790.f, 10.f}, {10.f, 590.f}})
    {
        Station s1(sf::Vector2f(200.f,400.f), 100.f, 0);
        Station s2(sf::Vector2f(600.f,300.f), 100.f, 1);
        Station s3(sf::Vector2f(10.f,590.f), 100.f, 2);

        stations = std::vector<Station> ({s1, s2, s3});

        camera = window.getView();
        uiView = window.getView();
        window.setView(camera);
        greyPause.setSize(maxSpeedZoom * sf::Vector2f{static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
        greyPause.setOrigin(0.5f * greyPause.getSize());
        greyPause.setPosition(0.5f * camera.getSize());
        std::cout << camera.getCenter().x << ", " << camera.getCenter().y << std::endl;
        greyPause.setFillColor(sf::Color{255, 255, 255, 128});
        initializeGame();
    }
    LoopLine::~LoopLine()
    {
    }

    void LoopLine::initializeGame()
    {
        srand (time(NULL));

        textureManager.loadTexture("assets/images/train.png", "train_spritesheet");
        textureManager.loadTexture("assets/images/station1.png", "station1_spritesheet");
        rails.train.setSprite(textureManager.getTexture("train_spritesheet"), sf::IntRect{0, 0, 100, 60}, {60.f, 12.f});

        for(auto& station : stations)
        {
            station.setSprite(textureManager.getTexture("station1_spritesheet"), sf::IntRect{0, 0, 150, 107});
            station.setSpritePosition(station.position);
        }

        inputManager.addEventCommand(rails.train.accel, sf::Keyboard::W);
        inputManager.addEventCommand(rails.train.deaccel, sf::Keyboard::S);
        inputManager.addReleaseEventCommand(rails.train.noaccel, sf::Keyboard::W);
        inputManager.addReleaseEventCommand(rails.train.noaccel, sf::Keyboard::S);

        inputManager.addEventCommand(std::make_shared<loopline::LambdaCommand>([this]() { window.close(); }), sf::Keyboard::Escape);
        inputManager.addEventCommand(std::make_shared<loopline::LambdaCommand>([this]()
        {
            if(state == GAME) state = PAUSE;
            else if(state == PAUSE) state = GAME;
        }), sf::Keyboard::P);
        inputManager.addEventCommand(std::make_shared<loopline::LambdaCommand>([this]() { debug = !debug; }), sf::Keyboard::F1);

        textFont.loadFromFile("assets/COOPBL.TTF");
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

    void LoopLine::resizeWindow()
    {
        camera.setSize({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
        uiView.setSize({static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
        window.setView(camera);
        greyPause.setSize(maxSpeedZoom * sf::Vector2f{static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});
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
            case sf::Event::Resized:
                resizeWindow();
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
        switch(state)
        {
        case MENU:
            break;
        case GAME:
            inputManager.update(deltaTime);
            rails.update(deltaTime);
            break;
        case PAUSE:
            break;
        }
    }

    void LoopLine::fixedUpdate(sf::Time const &deltaTime)
    {
        switch(state)
        {
        case MENU:
            break;
        case GAME:
            inputManager.fixedUpdate(deltaTime);
            rails.fixedUpdate(deltaTime);
            spawnPassengers();

            if (rails.train.speed <= 0.0001)
            {
                //check if within station radius
                for (auto &station : stations)
                {
                    if (vectorLength(rails.getWorldPosition(rails.train.railPosition) - station.position) < station.radius)
                    {
                        updatePassengers(station.id);
                    }
                }
            }
            break;
        case PAUSE:
            break;
        }
    }

    void LoopLine::render()
    {
        float speedZoom = 1.0f;
        switch(state)
        {
        case MENU:
            break;

        case GAME:
        case PAUSE:
            speedZoom = 1.0f + rails.train.speed / rails.train.maxSpeed * (maxSpeedZoom - 1.0f);
            camera.setSize(speedZoom * sf::Vector2f{static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});

            camera.setCenter(rails.getWorldPosition(rails.train.railPosition));

            window.setView(camera);

            // clear the window with black color
            window.clear(sf::Color::Black);

            for (auto &station : stations)
            {
                station.draw(window);
            }

            rails.draw(window);

            if(debug) drawDebug();
            drawUI();

            if (state == PAUSE)
            {
                window.draw(greyPause);
            }

            break;
        }
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

    void LoopLine::drawUI()
    {
        window.setView(uiView);
        std::string text(std::to_string(gold) + " Gold");
        sf::Text goldText(text, textFont);
        goldText.setOrigin(goldText.findCharacterPos(text.size()-1));
        sf::Vector2f topRight = uiView.getCenter() + 0.5f * sf::Vector2f{uiView.getSize().x, -uiView.getSize().y};
        goldText.setPosition(topRight + sf::Vector2f{-50.f, 50.f - goldText.getCharacterSize()});
        goldText.setFillColor(sf::Color::Red);

        window.draw(goldText);

        window.setView(camera);
    }

    void LoopLine::drawDebug()
    {
        sf::CircleShape stationDot(25.f);
        stationDot.setFillColor(sf::Color::Red);
        stationDot.setOrigin(12.5f, 12.5f);

        sf::CircleShape controlDot(15.f);
        controlDot.setFillColor(sf::Color::Blue);
        controlDot.setOrigin(7.5f, 7.5f);

        sf::CircleShape railDot(10.f);
        railDot.setFillColor(sf::Color::White);
        railDot.setOrigin(5.f, 5.f);

        float railLength = rails.railLengths[rails.railLengths.size() - 1];
        for(float i = 0.0f; i < railLength; i += 0.25f)
        {
            railDot.setPosition(rails.getWorldPosition(i));
            window.draw(railDot);
        }
        for(int i = 0; i < stations.size(); ++i)
        {
            stationDot.setPosition(stations[i].position);
            window.draw(stationDot);
        }
        for(int i = 0; i < rails.controlPoints.size(); ++i)
        {
            controlDot.setPosition(rails.controlPoints[i]);
            window.draw(controlDot);
        }
    }
} // namespace loopline