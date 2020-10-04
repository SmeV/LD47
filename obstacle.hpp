#ifndef __LOOPLINE_OBSTACLE_HPP__
#define __LOOPLINE_OBSTACLE_HPP__

#include "updatable.hpp"
#include "drawable.hpp"

namespace loopline
{
    class Obstacle : public Updatable, public Drawable
    {
        private:
        public:
        enum Type{TRAFFICLIGHT, CAR, PERSON};
        enum State{APPROACH, BLOCKING, LEAVE};

        Obstacle();

        virtual void update(sf::Time const &deltaTime) override;
        virtual void fixedUpdate(sf::Time const &deltaTime) override;

        sf::Time fadingTime;
        float railPosition;
        sf::Time duration;
        sf::Time elapsed;
        Type type;
        State state;
    };
} // namespace loopline

#endif