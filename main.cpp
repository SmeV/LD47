#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include <map>
#include <vector>
#include <functional>

#include "rails.hpp"
#include "inputManager.hpp"
#include "train.hpp"

int main()
{
    // create and initialize the window (+open)
    sf::RenderWindow window(sf::VideoMode{800, 600}, "My window");

    int counter = 0;
    
    // this sets how much time has to pass for an update, in case more time passed, do several updates at once
    sf::Time updateTime = sf::milliseconds(10);

    // the clock to keep track of how much time has passed between a cycle
    sf::Clock clock;

    // this saves how much time has passed
    sf::Time elapsed = clock.restart();

    // keep track if we have to redraw things. No need if nothing has been updated!
    bool update = true;

    loopline::InputManager inputManager;
    loopline::Rails rails({{10.f, 10.f}, {790.f, 10.f}, {10.f, 590.f}});

    inputManager.addEventCommand(rails.train.accel, sf::Keyboard::W);
    inputManager.addEventCommand(rails.train.deaccel, sf::Keyboard::S);
    inputManager.addReleaseEventCommand(rails.train.noaccel, sf::Keyboard::W);
    inputManager.addReleaseEventCommand(rails.train.noaccel, sf::Keyboard::S);

    inputManager.addEventCommand(std::make_shared<loopline::LambdaCommand>([&window]() { window.close(); }), sf::Keyboard::Escape);


    // run the program as long as the window is open
    while (window.isOpen())
    {
        // add the time passed since the last cycle
        elapsed = elapsed + clock.restart();

        // check all the window's events that were triggered since the last iteration of the loop
        // for some events you probably don't want to apply the rule to wait for an update!
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

        // make as many updates as needed for the elapsed time
        while (elapsed > updateTime)
        {
            inputManager.fixedUpdate(updateTime);

            rails.fixedUpdate(updateTime);

            // don't forget to subtract the updateTime each cycle ;-)
            elapsed -= updateTime;
            update = true;
        }

        // only redraw if there has been an update!
        if(update)
        {
            rails.update(elapsed);

            // clear the window with black color
            window.clear(sf::Color::Black);

            rails.draw(window);

            window.display();

            update = false;
        }
    }

    return 0;
}