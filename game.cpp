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
        std::vector<sf::Vector2f> stationPositions{{{2252.f, 3212.f}, {3612.f, 2932.f}, {4844.f, 3052.f}}};
        for(auto& stationPosition : stationPositions)
        {
            stationPosition -= 0.5f * sf::Vector2f{8000.f, 4000.f};
        }

        Station s1(stationPositions[0], 300.f, 0);
        Station s2(stationPositions[1], 300.f, 1);
        Station s3(stationPositions[2], 300.f, 2);

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
        obstacleRails.reserve(3);
        nonObstacleRails.reserve(7);

        textFont.loadFromFile("assets/COOPBL.TTF");
        textureManager.loadTexture("assets/images/train.png", "train_spritesheet");
        textureManager.loadTexture("assets/images/waggon.png", "waggon_spritesheet");
        textureManager.loadTexture("assets/images/station1.png", "station1_spritesheet");
        textureManager.loadTexture("assets/images/bg.png", "worldmap");
        textureManager.loadTexture("assets/images/mbg.png", "middlemap");
        textureManager.loadTexture("assets/images/vbg.png", "frontmap");
        textureManager.loadTexture("assets/images/cars_100x60.png", "car_spritesheet");
        textureManager.loadTexture("assets/images/witch.png", "witch_spritesheet");
        textureManager.loadTexture("assets/images/fliegzeug_177x120.png", "birds_spritesheet");
        textureManager.loadTexture("assets/images/plane.png", "plane_spritesheet");
        textureManager.loadTexture("assets/images/wolken_109x91.png", "wolken_spritesheet");
        textureManager.loadTexture("assets/images/crash.png", "crash");

        crashSprite.setTexture(textureManager.getTexture("crash"));
        crashTextcopy.setFont(textFont);
        crashTextcopy.setCharacterSize(25U);

        worldMap.setTexture(textureManager.getTexture("worldmap"));
        worldMap.setOrigin(0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)});
        worldMap.setPosition(sf::Vector2f{0.f, 0.f});

        middleMap.setTexture(textureManager.getTexture("middlemap"));
        middleMap.setOrigin(0.5f * sf::Vector2f{static_cast<float>(middleMap.getTextureRect().width), static_cast<float>(middleMap.getTextureRect().height)});
        middleMap.setPosition(sf::Vector2f{0.f, 0.f});

        frontMap.setTexture(textureManager.getTexture("frontmap"));
        frontMap.setOrigin(0.5f * sf::Vector2f{static_cast<float>(frontMap.getTextureRect().width), static_cast<float>(frontMap.getTextureRect().height)});
        frontMap.setPosition(sf::Vector2f{0.f, 0.f});

        for(auto& worldControl : worldControlPoints)
        {
            worldControl -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }
        rails.setControlPoints(worldControlPoints);
        rails.trains.resize(1);

        rails.trains[0].setSprite(textureManager.getTexture("train_spritesheet"), sf::IntRect{0, 0, 150, 90}, {90.f, 30.f});
        rails.trains[0].railPosition = 15530;

        copyWagon.setSprite(textureManager.getTexture("waggon_spritesheet"), sf::IntRect{0, 0, 150, 90}, {90.f, 30.f});
        wagonButton.setText("Buy Wagon");
        wagonButton.setButton({200.f, 70.f}, {0.f, 0.f});
        wagonButton.setBuyFunc([this]() { upgradeWagon(); });
        wagonButton.cost = 10;
        wagonButton.changePosition({120.f, 70.f});
        wagonButton.changeAnchor(uiView.getCenter() - 0.5f * uiView.getSize());

        speedButton.setText("Buy Speed");
        speedButton.setButton({200.f, 70.f}, {0.f, 0.f});
        speedButton.setBuyFunc([this]() { upgradeSpeed(); });
        speedButton.cost = 5;
        speedButton.changePosition({120.f, 70.f + 70.f + 20.f});
        speedButton.changeAnchor(uiView.getCenter() - 0.5f * uiView.getSize());

        accelButton.setText("Buy Accel");
        accelButton.setButton({200.f, 70.f}, {0.f, 0.f});
        accelButton.setBuyFunc([this]() { upgradeAccel(); });
        accelButton.cost = 1;
        accelButton.changePosition({120.f, 70.f + 140.f + 40.f});
        accelButton.changeAnchor(uiView.getCenter() - 0.5f * uiView.getSize());

        /// upgradeablestationpositions
        /// {4136.f, 1496.f}, {5544.f, 1820.f}, {1960.f, 2124.f}
        station1Button.setText("Buy Station");
        station1Button.setButton({200.f, 70.f}, {0.f, 0.f});
        station1Button.setBuyFunc([this]() { buyStation1(); });
        station1Button.cost = 50;
        station1Button.changePosition(sf::Vector2f{4136.f, 1496.f} - 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)});
        station1Button.changeAnchor(sf::Vector2f{0.f, 0.f});

        /// upgradeablestationpositions
        /// {4136.f, 1496.f}, {5544.f, 1820.f}, {1960.f, 2124.f}
        station2Button.setText("Buy Station");
        station2Button.setButton({200.f, 70.f}, {0.f, 0.f});
        station2Button.setBuyFunc([this]() { buyStation2(); });
        station2Button.cost = 100;
        station2Button.changePosition(sf::Vector2f{5544.f, 1820.f} - 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)});
        station2Button.changeAnchor(sf::Vector2f{0.f, 0.f});

        /// upgradeablestationpositions
        /// {4136.f, 1496.f}, {5544.f, 1820.f}, {1960.f, 2124.f}
        station3Button.setText("Buy Station");
        station3Button.setButton({200.f, 70.f}, {0.f, 0.f});
        station3Button.setBuyFunc([this]() { buyStation3(); });
        station3Button.cost = 150;
        station3Button.changePosition(sf::Vector2f{1960.f, 2124.f} - 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)});
        station3Button.changeAnchor(sf::Vector2f{0.f, 0.f});

        for(auto& station : stations)
        {
            station.setSprite(textureManager.getTexture("station1_spritesheet"), sf::IntRect{0, 0, 250, 180}, {125.f, 90.f});
            station.setSpritePosition(station.position);
        }

        /// obstacle points track right 
        std::vector<sf::Vector2f> obstaclePoints{{{8500.f, 2312.f},{5348.f, 2432}, {5376.f, 4501.f},{8500.f, 4501.f}}};
        for(auto& obstaclePoint : obstaclePoints)
        {
            obstaclePoint -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }

        obstacleRails.push_back(Rails{obstaclePoints, Rails::LINEAR});
        obstacleRails[0].trains.resize(1);
        obstacleRails[0].trains[0].maxSpeed = 1000.f;
        obstacleRails[0].trains[0].speed = 1000.f;
        obstacleRails[0].trains[0].setSprite(textureManager.getTexture("car_spritesheet"), sf::IntRect{0, 0, 100, 60}, sf::Vector2f{50.f, 30.f});

        /// obstacle points track center 
        std::vector<sf::Vector2f> obstaclePoints1 = {{{8000.f, 2312.f}, {5348.f, 2432.f}, {5360.f, 2788.f}, {2400.f, 2812.f}, {868.f, 4000.f},{8500.f, 4501.f}}};
        for(auto& obstaclePoint1 : obstaclePoints1)
        {
            obstaclePoint1 -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }

        obstacleRails.push_back(Rails{obstaclePoints1, Rails::LINEAR});
        obstacleRails[1].trains.resize(1);
        obstacleRails[1].trains[0].maxSpeed = 1000.f;
        obstacleRails[1].trains[0].speed = 1000.f;
        obstacleRails[1].trains[0].setSprite(textureManager.getTexture("car_spritesheet"), sf::IntRect{100, 0, 100, 60}, sf::Vector2f{50.f, 30.f});

        /// obstacle points track left ring 
        std::vector<sf::Vector2f> obstaclePoints2 = {{{1028.f, 2228.f}, {1232.f, 2272.f}, {1384.f, 2356.f}, {1520.f, 2468.f}, {1520.f, 2600.f}, {1440.f, 2692.f}, {1284.f, 2776.f}, {1136.f, 2808.f}, {976.f, 2828.f}, {792.f, 2764.f}, {616.f, 2672.f}, {480.f, 2536.f}, {532.f, 2400.f}, {644.f, 2320.f}, {760.f, 2264.f}, {880.f, 2240.f}}};
        for(auto& obstaclePoint2 : obstaclePoints2)
        {
            obstaclePoint2 -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }

        obstacleRails.push_back(Rails{obstaclePoints2, Rails::LINEAR});
        obstacleRails[2].trains.resize(1);
        obstacleRails[2].trains[0].maxSpeed = 1000.f;
        obstacleRails[2].trains[0].speed = 1000.f;
        obstacleRails[2].trains[0].setSprite(textureManager.getTexture("car_spritesheet"), sf::IntRect{200, 0, 100, 60}, sf::Vector2f{50.f, 30.f});

        //non obstacles
        std::vector<sf::Vector2f> nonObsPoints = {{{-100.f, 1000.f},{8100.f, 1000.f}}};
        for(auto& nonObsPoint : nonObsPoints)
        {
            nonObsPoint -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }
        nonObstacleRails.push_back(Rails{nonObsPoints, Rails::LINEAR});
        nonObstacleRails[0].trains.resize(1);
        nonObstacleRails[0].trains[0].maxSpeed = 1000.f;
        nonObstacleRails[0].trains[0].speed = 1000.f;
        nonObstacleRails[0].trains[0].setSprite(textureManager.getTexture("plane_spritesheet"), sf::IntRect{0, 0, 180, 112});

        nonObsPoints = {{{-100.f, 2000.f},{8100.f, 2000.f}}};
        for(auto& nonObsPoint : nonObsPoints)
        {
            nonObsPoint -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }
        nonObstacleRails.push_back(Rails{nonObsPoints, Rails::LINEAR});
        nonObstacleRails[1].trains.resize(1);
        nonObstacleRails[1].trains[0].maxSpeed = 600.f;
        nonObstacleRails[1].trains[0].speed = 600.f;
        nonObstacleRails[1].trains[0].setSprite(textureManager.getTexture("birds_spritesheet"), sf::IntRect{0, 0, 177, 120});

        nonObsPoints = {{{-100.f, 3000.f},{8100.f, 1000.f}}};
        for(auto& nonObsPoint : nonObsPoints)
        {
            nonObsPoint -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }
        nonObstacleRails.push_back(Rails{nonObsPoints, Rails::LINEAR});
        nonObstacleRails[2].trains.resize(1);
        nonObstacleRails[2].trains[0].maxSpeed = 600.f;
        nonObstacleRails[2].trains[0].speed = 600.f;
        nonObstacleRails[2].trains[0].setSprite(textureManager.getTexture("birds_spritesheet"), sf::IntRect{177, 0, 177, 120});

        nonObsPoints = {{{780.f, 645.f},{2322.f, 645.f}}};
        for(auto& nonObsPoint : nonObsPoints)
        {
            nonObsPoint -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }
        nonObstacleRails.push_back(Rails{nonObsPoints, Rails::LINEAR});
        nonObstacleRails[3].trains.resize(1);
        nonObstacleRails[3].trains[0].maxSpeed = 200.f;
        nonObstacleRails[3].trains[0].speed = 200.f;
        nonObstacleRails[3].trains[0].setSprite(textureManager.getTexture("witch_spritesheet"), sf::IntRect{0, 0, 356, 385});

        nonObsPoints = {{{-100.f, 500.f},{8100.f, 500.f}}};
        for(auto& nonObsPoint : nonObsPoints)
        {
            nonObsPoint -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }
        nonObstacleRails.push_back(Rails{nonObsPoints, Rails::LINEAR});
        nonObstacleRails[4].trains.resize(1);
        nonObstacleRails[4].trains[0].maxSpeed = 100.f;
        nonObstacleRails[4].trains[0].speed = 100.f;
        nonObstacleRails[4].trains[0].setSprite(textureManager.getTexture("wolken_spritesheet"), sf::IntRect{0, 0, 109, 91});

        nonObsPoints = {{{-100.f, 1300.f},{8100.f, 1300.f}}};
        for(auto& nonObsPoint : nonObsPoints)
        {
            nonObsPoint -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }
        nonObstacleRails.push_back(Rails{nonObsPoints, Rails::LINEAR});
        nonObstacleRails[5].trains.resize(1);
        nonObstacleRails[5].trains[0].maxSpeed = 200.f;
        nonObstacleRails[5].trains[0].speed = 200.f;
        nonObstacleRails[5].trains[0].setSprite(textureManager.getTexture("wolken_spritesheet"), sf::IntRect{109, 0, 109, 91});

        nonObsPoints = {{{-100.f, 1600.f},{8100.f, 1600.f}}};
        for(auto& nonObsPoint : nonObsPoints)
        {
            nonObsPoint -= 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)};
        }
        nonObstacleRails.push_back(Rails{nonObsPoints, Rails::LINEAR});
        nonObstacleRails[6].trains.resize(1);
        nonObstacleRails[6].trains[0].maxSpeed = 300.f;
        nonObstacleRails[6].trains[0].speed = 300.f;
        nonObstacleRails[6].trains[0].setSprite(textureManager.getTexture("wolken_spritesheet"), sf::IntRect{2018, 0, 109, 91});

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

        crashClock.restart();
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
        speedButton.changeAnchor(uiView.getCenter() - 0.5f * uiView.getSize());
        accelButton.changeAnchor(uiView.getCenter() - 0.5f * uiView.getSize());
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
            for(auto& noRails : nonObstacleRails)
            {
                noRails.update(deltaTime);
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

            for(auto& noRails : nonObstacleRails)
            {
                noRails.fixedUpdate(deltaTime);
            }

            spawnPassengers(deltaTime);
            updatePassengers(deltaTime);
            checkCrash(deltaTime);

            for(auto& crashCloud : crashClouds)
            {
                crashCloud.second -= deltaTime;
                crashCloud.first.move(sf::Vector2f{0.f, -25.f * deltaTime.asSeconds()});
            }
            for(auto& crashText : crashTexts)
            {
                crashText.second -= deltaTime;
                crashText.first.move(sf::Vector2f{0.f, -25.f * deltaTime.asSeconds()});
            }

            break;
        case PAUSE:
            break;
        }
        crashClouds.erase(std::remove_if(crashClouds.begin(), crashClouds.end(), [] (std::pair<sf::Sprite, sf::Time> p) {return p.second < sf::seconds(0);}), crashClouds.end());
        crashTexts.erase(std::remove_if(crashTexts.begin(), crashTexts.end(), [] (std::pair<sf::Text, sf::Time> p) {return p.second < sf::seconds(0);}), crashTexts.end());
    }

    void LoopLine::mouseUpdate()
    {
        bool intersected = false;
        // menu update TODO?

        if(intersected) return;

        // ui update
        auto mousePos = uiView.getCenter() - 0.5f * uiView.getSize() + sf::Vector2f(sf::Mouse::getPosition(window));

        intersected = wagonButton.mouseUpdate(mousePos);
        intersected = speedButton.mouseUpdate(mousePos);
        intersected = accelButton.mouseUpdate(mousePos);

        if(intersected) return;

        // game update
        mousePos = camera.getCenter() - 0.5f * camera.getSize() + sf::Vector2f(sf::Mouse::getPosition(window));
        station1Button.mouseUpdate(mousePos);
        station2Button.mouseUpdate(mousePos);
        station3Button.mouseUpdate(mousePos);
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

            for(auto& noRails : nonObstacleRails)
            {
                noRails.draw(window);
            }

            window.draw(middleMap);

            rails.draw(window);

            for(auto& crashCloud : crashClouds)
            {
                window.draw(crashCloud.first);
            };
            for(auto& crashText : crashTexts)
            {
                window.draw(crashText.first);
            };

            window.draw(frontMap);

            station1Button.drawUI(window, textFont);
            station2Button.drawUI(window, textFont);
            station3Button.drawUI(window, textFont);

            drawInfo();

            if(debug) drawDebug();
            drawUI();

            if (state == PAUSE)
            {
                window.setView(uiView);
                window.draw(greyPause);
                window.setView(camera);

                sf::Text pauseText{"PAUSE", textFont, 100};
                pauseText.setOrigin( 0.5f * sf::Vector2f(pauseText.getLocalBounds().width, pauseText.getLocalBounds().height) );
                pauseText.setPosition(camera.getCenter());
                window.draw(pauseText);
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
                if(distance <= crashRadius) crash(obstacle, rail.getWorldPosition(obstacle.railPosition));
            }
        }
    }

    void LoopLine::crash(Train &crashedInto, sf::Vector2f const &position)
    {
        if(crashClock.getElapsedTime() < crashCooldown) return;

        crashSprite.setPosition(position - sf::Vector2f{0.f, 100.f});
        crashClouds.push_back(std::make_pair(crashSprite, sf::seconds(5)));

        int fledPassengers = static_cast<int>(rand() % (rails.trains[0].passengers+1));
        sf::Text crashText{"          CRASH!\n" + std::to_string(fledPassengers) + " passengers fled your train to survive!", textFont, 25U};
        crashText.setOrigin(0.5f * sf::Vector2f{crashText.getLocalBounds().width, crashText.getLocalBounds().height});
        crashText.setPosition(position - sf::Vector2f{0.f, 150.f});
        crashText.setColor(sf::Color::Red);
        crashTexts.push_back(std::make_pair(crashText, sf::seconds(5)));
            
        rails.trains[0].passengers -= fledPassengers;

        for(auto& passenger : passengers)
        {
            if(fledPassengers == 0) break;

            if(passenger.boardedTrain && !passenger.killPassenger)
            {
                passenger.killPassenger = true;
                fledPassengers--;
            }
        }

        //crashText.push_back(std::mae)
        crashClock.restart();
        // TODO: CRASH!!!
        //rails.trains[0].addWagon(copyWagon);
        //crashedInto.setSprite(textureManager.getTexture("car_spritesheet"),  sf::IntRect{63, 0, 63, 37}, sf::Vector2f{31.5f, 23.5f});
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
        speedButton.drawUI(window, textFont);
        accelButton.drawUI(window, textFont);

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

        for(auto& noRails : nonObstacleRails)
        {
            railLength = noRails.railLengths[noRails.railLengths.size() - 1];
            for (float i = 0.0f; i < railLength; i += 2.f)
            {
                railDot.setPosition(noRails.getWorldPosition(i));
                window.draw(railDot);
            }
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

    void LoopLine::upgradeSpeed()
    {
        if(gold >= speedButton.cost)
        {
            gold -= speedButton.cost;
            rails.trains[0].maxSpeed *= 1.25f;
            speedButton.cost *= 2;
        }
    }

    void LoopLine::upgradeAccel()
    {
        if(gold >= accelButton.cost)
        {
            gold -= accelButton.cost;
            rails.trains[0].accel->accel *= 1.5f;
            rails.trains[0].deaccel->accel *= 1.5f;
            accelButton.cost *= 2;
        }
    }

        /// upgradeablestationpositions
        /// {4136.f, 1496.f}, {5544.f, 1820.f}, {1960.f, 2124.f}
    void LoopLine::buyStation1()
    {
        if(gold >= station1Button.cost)
        {
            gold -= station1Button.cost;
            Station newStation{sf::Vector2f{4136.f, 1496.f} - 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)}, 300, 3};
            newStation.setSprite(textureManager.getTexture("station1_spritesheet"), sf::IntRect{0, 0, 250, 180}, {125.f, 90.f});
            newStation.setSpritePosition(newStation.position);
            stations.push_back( newStation );
            station1Button.active = false;
        }
    }
    void LoopLine::buyStation2()
    {
        if(gold >= station2Button.cost)
        {
            gold -= station2Button.cost;
            Station newStation{sf::Vector2f{5544.f, 1820.f} - 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)}, 300, 3};
            newStation.setSprite(textureManager.getTexture("station1_spritesheet"), sf::IntRect{0, 0, 250, 180}, {125.f, 90.f});
            newStation.setSpritePosition(newStation.position);
            stations.push_back( newStation );
            station2Button.active = false;
        }
    }
    void LoopLine::buyStation3()
    {
        if(gold >= station3Button.cost)
        {
            gold -= station3Button.cost;
            Station newStation{sf::Vector2f{1960.f, 2124.f} - 0.5f * sf::Vector2f{static_cast<float>(worldMap.getTextureRect().width), static_cast<float>(worldMap.getTextureRect().height)}, 300, 3};
            newStation.setSprite(textureManager.getTexture("station1_spritesheet"), sf::IntRect{0, 0, 250, 180}, {125.f, 90.f});
            newStation.setSpritePosition(newStation.position);
            stations.push_back( newStation );
            station3Button.active = false;
        }
    }
} // namespace loopline