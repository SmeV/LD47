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
        float speed;
        float acceleration;

        sf::Vector2f position;

        sf::Sprite sprite;
        sf::Texture slimeTexture;

    public:
        Train();
        ~Train();

        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);
        virtual void draw(sf::RenderWindow &window);

        std::shared_ptr<LambdaCommand> accel, deaccel, chime, noaccel;
    };
} // namespace loopline

#endif