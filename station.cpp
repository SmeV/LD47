#include "station.hpp"

namespace loopline
{
    Station::Station(){}
    Station::Station(sf::Vector2f const& pos, float r, int i)
        : position(pos), radius(r), id(i)
    {
    }

    Station::~Station()
    {
    }

    void Station::drawInfo(sf::RenderWindow &window, sf::Font const &font) const
    {
        std::string infoText = std::to_string(waitingPassengers) + " / " + std::to_string(maxCapacity) + " waiting";
        sf::Text info{infoText, font, 15U};
        info.setOrigin(0.5f * sf::Vector2f{static_cast<float>(info.getLocalBounds().width), static_cast<float>(info.getLocalBounds().height)});
        info.setPosition(position + sf::Vector2f{0.f, 100.f});
        info.setFillColor(waitingPassengers == maxCapacity ? sf::Color::Red : sf::Color::White);

        window.draw(info);
    }

} // namespace loopline
