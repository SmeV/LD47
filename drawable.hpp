#ifndef __LOOPLINE_DRAWABLE_HPP__
#define __LOOPLINE_DRAWABLE_HPP__

#include "SFML/Graphics.hpp"

namespace loopline
{

    class Drawable
    {
    private:

    public:
        Drawable();
        virtual ~Drawable();

        virtual void setSprite(sf::Texture const &tex, sf::IntRect const &rect, sf::Vector2f const &origin = {0.f, 0.f});
        virtual void setSpritePosition(sf::Vector2f const &worldPos);
        virtual void setSpriteRotation(float angle);
        virtual void setSpriteScale(sf::Vector2f const& scale);

        virtual void draw(sf::RenderWindow &window) const;
        virtual void drawInfo(sf::RenderWindow &window, sf::Font const &font) const;
        virtual void drawUI(sf::RenderWindow &window, sf::Font const &font) const;

        sf::Sprite sprite;
    };
} // namespace loopline

#endif