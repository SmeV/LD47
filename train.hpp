#ifndef __LOOPLINE_TRAIN_HPP__
#define __LOOPLINE_TRAIN_HPP__

#include <SFML/Graphics.hpp>

#include "command.hpp"
#include "updatable.hpp"
#include "drawable.hpp"

namespace loopline
{
    class Train : public Updatable, public Drawable
    {
    private:
        friend class Rails;
        float acceleration;


        float length;

    public:
        Train();
        ~Train();

        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);

        std::shared_ptr<LambdaCommand> accel, deaccel, chime, noaccel;
        float railPosition;
        float speed;
        float maxSpeed = 500.0f;
    };
} // namespace loopline

#endif