#include "drawable.hpp"

namespace loopline
{
    Drawable::Drawable()
    {
    }

    Drawable::~Drawable()
    {
    }

    void Drawable::setSprite(sf::Texture const &tex, sf::IntRect const &rect, sf::Vector2f const &origin)
    {
        sprite.setTexture(tex);
        sprite.setTextureRect(rect);
        sprite.setOrigin(origin);
    }

    void Drawable::setSpritePosition(sf::Vector2f const &worldPos)
    {
        sprite.setPosition(worldPos);
    }

    void Drawable::setSpriteRotation(float angle)
    {
        sprite.setRotation(angle);
    }

    void Drawable::setSpriteScale(sf::Vector2f const &scale)
    {
        sprite.setScale(scale);
    }

    void Drawable::draw(sf::RenderWindow &window) const
    {
        window.draw(this->sprite);
    }

    void Drawable::drawInfo(sf::RenderWindow &window, sf::Font const &font) const
    {
    }

    void Drawable::drawUI(sf::RenderWindow &window, sf::Font const &font) const
    {
    }

} // namespace loopline