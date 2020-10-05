#ifndef __LOOPLINE_UPGRADE_HPP__
#define __LOOPLINE_UPGRADE_HPP__

#include "button.hpp"

namespace loopline
{
    class Upgrade : public Button
    {
    private:
    public:
        Upgrade(std::string const &text = "", int cost = 0, std::function<void()> const &buyFnc = [](){});

        void upgrade();

        void setBuyFunc(std::function<void()> const &buyFunc);
        void changeAnchor(sf::Vector2f const &anchor);
        void changePosition(sf::Vector2f const &pos);

        virtual void update(sf::Time const &deltaTime) override;
        virtual void drawUI(sf::RenderWindow &window, sf::Font const &font) const override;

        int cost;
        std::function<void()> buyFnc;
        sf::Vector2f anchorPoint = {0.f, 0.f};
    };
} // namespace loopline

#endif