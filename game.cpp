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
        Station s1(sf::Vector2f(200.f,400.f), 300.f, 0);
        Station s2(sf::Vector2f(600.f,300.f), 300.f, 1);
        Station s3(sf::Vector2f(10.f,590.f), 300.f, 2);

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

        obstacleRails.push_back(Rails{{{-3999.f, 0.f},{4001.f, 0.f}}, Rails::LINEAR});
        obstacleRails[0].trains.resize(1);
        obstacleRails[0].trains[0].maxSpeed = 1000.f;
        obstacleRails[0].trains[0].speed = 1000.f;


        textureManager.loadTexture("assets/images/train.png", "train_spritesheet");
        textureManager.loadTexture("assets/images/station1.png", "station1_spritesheet");
        textureManager.loadTexture("assets/images/bg.png", "worldmap");
        textureManager.loadTexture("assets/images/cars.png", "car_spritesheet");

        obstacleRails[0].trains[0].setSprite(textureManager.getTexture("car_spritesheet"), sf::IntRect{0, 0, 63, 37}, sf::Vector2f{31.5f, 23.5f});

        worldMap.setTexture(textureManager.getTexture("worldmap"));
        worldMap.setOrigin(0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)});
        worldMap.setPosition(sf::Vector2f{0.f, 0.f});


        for(auto& worldControl : worldControlPoints)
        {
            worldControl -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }
        rails.setControlPoints(worldControlPoints);
        rails.trains.resize(1);

        rails.trains[0].setSprite(textureManager.getTexture("train_spritesheet"), sf::IntRect{0, 0, 100, 60}, {60.f, 12.f});

        copyWagon.setSprite(textureManager.getTexture("train_spritesheet"), sf::IntRect{0, 0, 100, 60}, {60.f, 12.f});
        wagonButton.setText("Buy Wagon");
        wagonButton.setButton({200.f, 70.f}, {0.f, 0.f});
        wagonButton.setBuyFunc([this]() { upgradeWagon(); });
        wagonButton.cost = 10;

        wagonButton.changePosition({120.f, 70.f});
        wagonButton.changeAnchor(uiView.getCenter() - 0.5f * uiView.getSize());

        for(auto& station : stations)
        {
            station.setSprite(textureManager.getTexture("station1_spritesheet"), sf::IntRect{0, 0, 150, 107}, {75.f, 53.5f});
            station.setSpritePosition(station.position);
        }

        //inputManager.addEventCommand(rails.trains[0].accel, sf::Keyboard::W);
        inputManager.addEventCommand(rails.trains[0].accel, sf::Keyboard::W);
        inputManager.addEventCommand(rails.trains[0].deaccel, sf::Keyboard::S);
        inputManager.addReleaseEventCommand(rails.trains[0].noaccel, sf::Keyboard::W);
        inputManager.addReleaseEventCommand(rails.trains[0].noaccel, sf::Keyboard::S);

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

        // make a mouse update
        mouseUpdate();

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
        greyPause.setSize(uiView.getSize());
        greyPause.setOrigin(0.5f * greyPause.getSize());
        greyPause.setPosition(uiView.getCenter());

        wagonButton.changeAnchor(uiView.getCenter() - 0.5f * uiView.getSize());
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
            for(auto& oRails : obstacleRails)
            {
                oRails.update(deltaTime);
            }
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
            for(auto& oRails : obstacleRails)
            {
                oRails.fixedUpdate(deltaTime);
            }

            spawnPassengers(deltaTime);
            updatePassengers(deltaTime);
            checkCrash(deltaTime);

            break;
        case PAUSE:
            break;
        }
    }

    void LoopLine::mouseUpdate()
    {
        bool intersected = false;
        // menu update TODO?

        if(intersected) return;

        // ui update
        auto mousePos = uiView.getCenter() - 0.5f * uiView.getSize() + sf::Vector2f(sf::Mouse::getPosition(window));

        intersected = wagonButton.mouseUpdate(mousePos);

        if(intersected) return;

        // game update
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
            speedZoom = 1.0f + rails.trains[0].speed / rails.trains[0].maxSpeed * (maxSpeedZoom - 1.0f);
            camera.setSize(speedZoom * sf::Vector2f{static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)});

            camera.setCenter(rails.getWorldPosition(rails.trains[0].railPosition));

            window.setView(camera);

            // clear the window with black color
            window.clear(sf::Color::Black);

            window.draw(worldMap);

            for (auto &station : stations)
            {
                station.draw(window);
            }

            for(auto& oRails : obstacleRails)
            {
                oRails.draw(window);
            }
            rails.draw(window);

            drawInfo();

            if(debug) drawDebug();
            drawUI();

            if (state == PAUSE)
            {
                window.setView(uiView);
                window.draw(greyPause);
                window.setView(camera);
            }

            break;
        }
        window.display();
    }

    void LoopLine::spawnPassengers(sf::Time const &deltaTime)
    {
        if(passengers.size() < maxPassengers && rand() % 1000 < (1000*deltaTime.asSeconds()))
        {
            int in = rand() % stations.size();
            int inPlus = 1 + (rand() % (stations.size()-1));
            int out = (in+inPlus) % stations.size();

            if(stations[in].waitingPassengers < stations[in].maxCapacity)
            {
                passengers.push_back(Passenger(in, out));
                stations[in].waitingPassengers++;
            }
        }
    }

    void LoopLine::updatePassengers(sf::Time const &deltaTime)
    {
        
        int stoppedAtStation = -1;
        float stoppedDistance = 100000.f;
        if (rails.trains[0].state == Train::STOPPED)
        {
            for (int i=0; i < stations.size(); ++i)
            {
                float distance = vectorLength(rails.getWorldPosition(rails.trains[0].railPosition) - stations[i].position);
                if (distance < stations[i].radius && distance < stoppedDistance)
                {
                    stoppedAtStation = i;
                    stoppedDistance = distance;
                }
            }
        }

        for(auto& passenger : passengers)
        {
            passenger.fixedUpdate(deltaTime);

            if(!passenger.boardedTrain && !passenger.killPassenger && rails.trains[0].passengers < rails.trains[0].maxCapacity && passenger.enterStation == stoppedAtStation)
            {
                passenger.boardedTrain = true;
                passenger.waitingTime.restart();
                stations[stoppedAtStation].waitingPassengers--;
                rails.trains[0].passengers++;
            }

            if(passenger.boardedTrain == true && passenger.exitStation == stoppedAtStation)
            {
                passenger.killPassenger = true;
                rails.trains[0].passengers--;

                gold += passenger.payTicket();
            }

            if(!passenger.boardedTrain && passenger.killPassenger)
            {
                stations[passenger.enterStation].waitingPassengers--;
            }
        }

        passengers.erase(std::remove_if(passengers.begin(), passengers.end(), [] (Passenger p) {return p.killPassenger;}), passengers.end());

    }

    void LoopLine::checkCrash(sf::Time const &deltaTime)
    {
        sf::Vector2f ourTrainPosition = rails.getWorldPosition(rails.trains[0].railPosition);
        for(auto &rail : obstacleRails)
        {
            for(auto &obstacle : rail.trains)
            {
                sf::Vector2f obstaclePosition = rail.getWorldPosition(obstacle.railPosition);

                float distance = vectorLength(ourTrainPosition - obstaclePosition);
                if(distance <= crashRadius) crash(obstacle);
            }
        }
    }

    void LoopLine::crash(Train &crashedInto)
    {
        // TODO: CRASH!!!
        rails.trains[0].addWagon(copyWagon);
        crashedInto.setSprite(textureManager.getTexture("car_spritesheet"),  sf::IntRect{63, 0, 63, 37}, sf::Vector2f{31.5f, 23.5f});
        //state = PAUSE;
    }

    void LoopLine::drawUI()
    {
        window.setView(uiView);
        std::string text(std::to_string(gold) + " Gold");
        sf::Text goldText(text, textFont);
        goldText.setOrigin(goldText.findCharacterPos(text.size()-1) + 0.5f * sf::Vector2f{static_cast<float>(goldText.getCharacterSize()), static_cast<float>(goldText.getCharacterSize())});
        sf::Vector2f topRight = uiView.getCenter() + 0.5f * sf::Vector2f{uiView.getSize().x, -uiView.getSize().y};
        goldText.setPosition(topRight + sf::Vector2f{-30.f, 30.f});
        goldText.setFillColor(sf::Color::Red);

        window.draw(goldText);
        rails.trains[0].drawUI(window, textFont);

        wagonButton.drawUI(window, textFont);

        window.setView(camera);
    }

    void LoopLine::drawInfo()
    {
        for(auto& station : stations)
        {
            station.drawInfo(window, textFont);
        }
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
        for(float i = 0.0f; i < railLength; i += 2.f)
        {
            railDot.setPosition(rails.getWorldPosition(i));
            window.draw(railDot);
        }
        for(auto& oRails : obstacleRails)
        {
            railLength = oRails.railLengths[oRails.railLengths.size() - 1];
            for (float i = 0.0f; i < railLength; i += 2.f)
            {
                railDot.setPosition(oRails.getWorldPosition(i));
                window.draw(railDot);
            }
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

    void LoopLine::upgradeWagon()
    {
        if(gold >= wagonButton.cost)
        {
            gold -= wagonButton.cost;
            rails.trains[0].addWagon(copyWagon);
            wagonButton.cost *= 2;
        }
    }
} // namespace loopline