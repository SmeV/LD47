#ifndef __LOOPLINE_DRAWABLE_HPP__
#define __LOOPLINE_DRAWABLE_HPP__

#include "SFML/Graphics.hpp"

namespace loopline
{

    class Drawable
    {
    private:
        sf::Sprite sprite;

    public:
        Drawable();
        virtual ~Drawable();

        virtual void setSprite(sf::Texture const &tex, sf::IntRect const &rect, sf::Vector2f const &origin = {0.f, 0.f});
        virtual void setSpritePosition(sf::Vector2f const &worldPos);
        virtual void setSpriteRotation(float angle);
        virtual void setSpriteScale(sf::Vector2f const& scale);

        virtual void draw(sf::RenderWindow &window) const;
    };
} // namespace loopline

#endif