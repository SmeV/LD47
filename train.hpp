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

    public:
        Train();
        ~Train();

        void setWorldposition(sf::Vector2f const &worldPos);
        void setSprite(sf::Texture const& tex, sf::IntRect const& rect, sf::Vector2f const &origin = {0.f, 0.f});

        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);
        virtual void draw(sf::RenderWindow &window) const;

        std::shared_ptr<LambdaCommand> accel, deaccel, chime, noaccel;
    };
} // namespace loopline

#endif