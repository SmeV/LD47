#ifndef __LOOPLINE_HELPER_HPP__
#define __LOOPLINE_HELPER_HPP__

#include <math.h>
#include <SFML/Graphics.hpp>

sf::Vector2f CubicInterpolate(
  sf::Vector2f y0, sf::Vector2f y1,
    sf::Vector2f y2, sf::Vector2f y3,
   float mu);

float vectorLength(sf::Vector2f v);

#endif