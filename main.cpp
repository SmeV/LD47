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
#include "game.hpp"

int main()
{
    loopline::LoopLine game;

    game.start();


    return 0;
}