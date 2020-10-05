#include "button.hpp"

namespace loopline
{
        Button::Button(sf::Vector2f const &size, sf::Vector2f const &pos)
            : size(size), position(pos), button(size), textNormal(sf::Color::Red), textHover(sf::Color::Red), textClick(sf::Color::Blue), buttonNormal(sf::Color{255,255,255,128}), buttonHover(sf::Color{255,255,0,128}), buttonClick(sf::Color::Red)
        {
            button.setOrigin(0.5f * size);
            button.setPosition(pos);

        }

        bool Button::isHovered(sf::Vector2f const &mousePos) const
        {
            return button.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)});
        }
        void Button::click() const
        {
            for(auto& func : clickFunctions)
            {
                func();
            }
        }

        void Button::setButton(sf::Vector2f const &size, sf::Vector2f const &pos)
        {
            this->size = size;
            this->position = pos;
            button.setSize(size);
            button.setOrigin(0.5f * size);
            button.setPosition(pos);

            setText(text);
        }

        void Button::setText(std::string const &text)
        {
            this->text = text;
        }

        bool Button::mouseUpdate(sf::Vector2f const &mousePos)
        {
            if(!active) return false;
            auto oldState = state;
            if(isHovered(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                state = CLICK;
            }
            else if(isHovered(mousePos))
            {
                state = HOVER;
            }
            else
            {
                state = NORMAL;
            }
            
            switch(state)
            {
            case NORMAL:
                button.setFillColor(buttonNormal);
                buttonText.setFillColor(textNormal);
                break;
            case HOVER:
                button.setFillColor(buttonHover);
                buttonText.setFillColor(textHover);
                break;
            case CLICK:
                button.setFillColor(buttonClick);
                buttonText.setFillColor(textClick);
                break;
            }

            if(state == CLICK && oldState != state)
            {
                click();
            }

            return isHovered(mousePos);
        }

        void Button::drawUI(sf::RenderWindow &window, sf::Font const &font) const
        {
            if(!active) return;
            sf::Text drawText{text, font};
            drawText.setOrigin(0.5f * sf::Vector2f{drawText.getLocalBounds().width, drawText.getLocalBounds().height} + sf::Vector2f{0.f, 10.f});
            drawText.setFont(font);
            drawText.setPosition(button.getPosition());

            window.draw(button);
            window.draw(drawText);
        }
}