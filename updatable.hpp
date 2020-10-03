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

    private:
    };
}; // namespace loopline

#endif