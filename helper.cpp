#include "helper.hpp"

sf::Vector2f CubicInterpolate(
  sf::Vector2f y0, sf::Vector2f y1,
    sf::Vector2f y2, sf::Vector2f y3,
   float mu)
{
    sf::Vector2f a0,a1,a2,a3;
    float mu2;

   mu2 = mu*mu;
   a0 = y3 - y2 - y0 + y1;
   a1 = y0 - y1 - a0;
   a2 = y2 - y0;
   a3 = y1;

   return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}

float vectorLength(sf::Vector2f v)
{
  return sqrtf(powf(v.x, 2.f) + powf(v.y, 2.f));
}
