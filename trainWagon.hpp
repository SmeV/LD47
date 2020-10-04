#ifndef __LOOPLINE_TRAINWAGON_HPP__
#define __LOOPLINE_TRAINWAGON_HPP__

#include "updatable.hpp"
#include "drawable.hpp"

namespace loopline
{
    class TrainWagon : public Updatable, public Drawable
    {
        private:
        public:
        TrainWagon(float length = 120.f);
        ~TrainWagon();
        TrainWagon(TrainWagon const &wagon);

        virtual void update(sf::Time const &deltaTime) override;
        virtual void fixedUpdate(sf::Time const &deltaTime) override;

        float length;
    };
} // namespace loopline

#endif