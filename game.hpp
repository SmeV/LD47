#ifndef __LOOPLINE_GAME_HPP__
#define __LOOPLINE_GAME_HPP__

#include <SFML/Graphics.hpp>

#include "textureManager.hpp"
#include "inputManager.hpp"
#include "rails.hpp"

namespace loopline
{
    class LoopLine
    {
        private:
        TextureManager textureManager;
        InputManager inputManager;

        sf::RenderWindow window;

        // this sets how much time has to pass for an update, in case more time passed, do several updates at once
        sf::Time updateTime = sf::milliseconds(10);

        // the clock to keep track of how much time has passed between a cycle
        sf::Clock clock;

        // this saves how much time has passed
        sf::Time elapsed = clock.restart();


        Rails rails;

        public:
        LoopLine();
        ~LoopLine();

        void initializeGame();

        void start();
        void gameLoop();

        virtual void handleEvents();
        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);
        virtual void render();
    };
}

#endif