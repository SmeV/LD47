#ifndef __LOOPLINE_GAME_HPP__
#define __LOOPLINE_GAME_HPP__

#include <SFML/Graphics.hpp>

#include "textureManager.hpp"
#include "inputManager.hpp"
#include "rails.hpp"
#include "station.hpp"
#include "passenger.hpp"
#include "obstacle.hpp"

#include "upgrade.hpp"

namespace loopline
{
    class LoopLine
    {
        private:
        enum GameState{ MENU, GAME, PAUSE };

        TextureManager textureManager;
        InputManager inputManager;
        GameState state = GAME;

        sf::RenderWindow window;

        // this sets how much time has to pass for an update, in case more time passed, do several updates at once
        sf::Time updateTime = sf::milliseconds(10);

        // the clock to keep track of how much time has passed between a cycle
        sf::Clock clock;

        // this saves how much time has passed
        sf::Time elapsed = clock.restart();


        int gold = 0;
        sf::Font textFont;
        std::vector<Station> stations;

        std::vector<Passenger> passengers;
        int maxPassengers = 300;

        sf::View camera;
        sf::View uiView;
        float maxSpeedZoom = 1.25;
        Rails rails;

        float crashRadius = 50.f;
        sf::Time crashCooldown = sf::seconds(2);
        sf::Clock crashClock;

        std::vector<Rails> obstacleRails;
        std::vector<Rails> nonObstacleRails;
        std::vector<std::pair<sf::Sprite, sf::Time> > crashClouds;
        std::vector<std::pair<sf::Text, sf::Time> > crashTexts;

        sf::Sprite worldMap;
        sf::Sprite middleMap;
        sf::Sprite frontMap;
        sf::Sprite crashSprite;
        sf::Text crashTextcopy;
        std::vector<sf::Vector2f> worldControlPoints{ {1188.f, 2560.f}, {1491.f, 2000.f}, {1773.f, 1620.f}, {2091.f, 1416.f}, {2094.f, 1575.f}, {1668.f, 2000.f}, {1977.f, 2262.f}, {2124.f, 2268.f}, {2271.f, 2070.f}, {2289.f, 2421.f}, {2454.f, 2514.f}, {2964.f, 1947.f}, {3138.f, 2286.f}, {3618.f, 2187.f}, {4047.f, 1695.f}, {3624.f, 2000.f}, {3174.f, 2790.f}, {2703.f, 3240.f}, {3081.f, 3150.f}, {3405.f, 2982.f}, {3795.f, 3264.f}, {4110.f, 2484.f}, {4515.f, 2340.f}, {4650.f, 2652.f}, {5427.f, 2000.f}, {5688.f, 2187.f}, {5544.f, 2544.f}, {5655.f, 2649.f}, {5787.f, 2751.f}, {5985.f, 2754.f}, {6216.f, 2877.f}, {6573.f, 2997.f}, {6132.f, 3156.f}, {5505.f, 2922.f}, {4839.f, 3195.f}, {4650.f, 2652.f}, {4617.f, 1695.f}, {3882.f, 1218.f}, {3219.f, 1509.f}, {2703.f, 1575.f}, {2157.f, 2000.f}, {2208.f, 2670.f}, {2547.f, 2730.f}, {2130.f, 3156.f}, {1665.f, 3129.f}, {1785.f, 2652.f} };

        TrainWagon copyWagon;
        Upgrade wagonButton;
        Upgrade speedButton;
        Upgrade accelButton;
        Upgrade station1Button;
        Upgrade station2Button;
        Upgrade station3Button;

        bool debug = false;

        sf::RectangleShape greyPause;

        public:
        LoopLine();
        ~LoopLine();

        void initializeGame();

        void start();
        void gameLoop();

        void spawnPassengers(sf::Time const &deltaTime);
        void updatePassengers(sf::Time const &deltaTime);
        void resizeWindow();

        virtual void handleEvents();
        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);
        virtual void mouseUpdate();
        virtual void render();

        void checkCrash(sf::Time const &deltaTime);
        void crash(Train &crashedInto, sf::Vector2f const &position);

        void drawInfo();
        void drawUI();
        void drawDebug();

        void upgradeWagon();
        void upgradeSpeed();
        void upgradeAccel();

        void buyStation1();
        void buyStation2();
        void buyStation3();
    };
}

#endif