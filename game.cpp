#include "game.hpp"

#include <iostream>

namespace loopline
{

    LoopLine::LoopLine()
        : window(sf::VideoMode{800, 600}, "Loop Line!"), rails({{10.f, 10.f}, {200.f,400.f},{400.f,100.f},{600.f, 300.f},{790.f, 10.f}, {10.f, 590.f}})
    {
        camera = window.getView();
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
        textureManager.loadTexture("slime_monster_24x24/slime_monster_spritesheet.png", "slime_spritesheet");
        rails.train.setSprite(textureManager.getTexture("slime_spritesheet"), sf::IntRect{0, 48, 72, 24}, {60.f, 12.f});

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
            //camera.zoom(speedZoom);

            window.setView(camera);

            // clear the window with black color
            window.clear(sf::Color::Black);

            rails.draw(window);

            if(state == PAUSE)
            {
                window.draw(greyPause);
            }

            break;
        }
        window.display();
    }
} // namespace loopline