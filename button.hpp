#ifndef __LOOPLINE_BUTTON_HPP__
#define __LOOPLINE_BUTTON_HPP__

#include <functional>
#include <vector>

#include <SFML/Graphics.hpp>

#include "updatable.hpp"

namespace loopline
{
    class Button : public Updatable
    {
        private:
        public:
        enum State{NORMAL, HOVER, CLICK};
        Button(sf::Vector2f const &size = sf::Vector2f{0.f, 0.f}, sf::Vector2f const &pos = sf::Vector2f{0.f, 0.f}); 

        bool isHovered(sf::Vector2i const &mousePos) const;
        void click() const;

        void setButton(sf::Vector2f const &size = sf::Vector2f{0.f, 0.f}, sf::Vector2f const &pos = sf::Vector2f{0.f, 0.f});
        void setText(std::string const &text);

        virtual void mouseUpdate(sf::Vector2i const &mousePos) override;

        virtual void drawUI(sf::RenderWindow &window, sf::Font const &font) const;

        State state = NORMAL;

        sf::Vector2f size, position;
        std::string text;
        sf::Text buttonText;
        sf::RectangleShape button;
        sf::Color textNormal, textHover, textClick;
        sf::Color buttonNormal, buttonHover, buttonClick;

        std::vector<std::function<void()>> clickFunctions;
    };
} // namespace loopline

#endif