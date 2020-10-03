#include "game.hpp"

namespace loopline
{

    LoopLine::LoopLine()
        : window(sf::VideoMode{800, 600}, "My window"), rails({{10.f, 10.f}, {790.f, 10.f}, {10.f, 590.f}})
    {
        initializeGame();
    }
    LoopLine::~LoopLine()
    {
    }

    void LoopLine::initializeGame()
    {
        textureManager.loadTexture("slime_monster_24x24/slime_monster_spritesheet.png", "slime_spritesheet");
        rails.train.setSprite(textureManager.getTexture("slime_spritesheet"), sf::IntRect{0, 48, 72, 24});

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
    }

    void LoopLine::render()
    {
        // clear the window with black color
        window.clear(sf::Color::Black);

        rails.draw(window);

        window.display();
    }
} // namespace loopline