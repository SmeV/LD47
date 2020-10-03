#ifndef __LOOPLINE_TRAIN_HPP__
#define __LOOPLINE_TRAIN_HPP__

#include <SFML/Graphics.hpp>

#include "command.hpp"
#include "updatable.hpp"

namespace loopline
{
    class Train : public Updatable
    {
    private:
        friend class Rails;
        float speed;
        float acceleration;
        float maxSpeed = 500.0f;

        float railPosition;

        float length;
        sf::Sprite sprite;
        sf::Texture slimeTexture;

    public:
        Train();
        ~Train();

        void setWorldposition(sf::Vector2f const &worldPos);

        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);
        virtual void draw(sf::RenderWindow &window) const;

        std::shared_ptr<LambdaCommand> accel, deaccel, chime, noaccel;
    };
} // namespace loopline

#endif