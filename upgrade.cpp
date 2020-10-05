#include "upgrade.hpp"

namespace loopline
{
    Upgrade::Upgrade(std::string const &text, int cost, std::function<void()> const &buyFnc)
        : Button(sf::Vector2f{200.f, 60.f})
    {
        setText(text);
        clickFunctions.push_back(buyFnc);
    }

    void Upgrade::setBuyFunc(std::function<void()> const &buyFunc)
    {
        clickFunctions.empty();
        clickFunctions.push_back(buyFunc);
    }

    void Upgrade::changeAnchor(sf::Vector2f const &anchor)
    {
        anchorPoint = anchor;
        button.setPosition(position + anchor);
    }

     void Upgrade::changePosition(sf::Vector2f const &pos)
     {
         position = pos;
         button.setPosition(position + anchorPoint);
     }

    void Upgrade::update(sf::Time const &deltaTime)
    {

    }

    void Upgrade::drawUI(sf::RenderWindow &window, sf::Font const &font) const
    {
        Button::drawUI(window, font);

        sf::Text costText("Cost: " + std::to_string(cost), font, 15U);
        costText.setOrigin(costText.findCharacterPos(costText.getString().getSize()-1) + sf::Vector2f(costText.getCharacterSize(), costText.getCharacterSize()));
        costText.setPosition(button.getPosition() + 0.5f * button.getSize() - sf::Vector2f{5.f, 5.f});
        window.draw(costText);
    }
} // namespace loopline