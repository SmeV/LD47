#ifndef __LOOPLINE_UPDATABLE_HPP__
#define __LOOPLINE_UPDATABLE_HPP__

#include "SFML/System.hpp"

namespace loopline
{
    class Updatable
    {
    public:
        virtual void update(sf::Time const &deltaTime) {}
        virtual void fixedUpdate(sf::Time const &deltaTime) {}
        virtual bool mouseUpdate(sf::Vector2f const &mousePos) { return false; }

    private:
    };
}; // namespace loopline

#endif