#ifndef __LOOPLINE_RAILS_HPP__
#define __LOOPLINE_RAILS_HPP__

#include <vector>

#include <SFML/Graphics.hpp>

#include "train.hpp"

namespace loopline
{
    class Rails : public Updatable
    {
        private:

        public:
        Rails(std::vector<sf::Vector2f> const &pts = {});
        ~Rails();

        sf::Vector2f getWorldPosition(float railPosition) const;

        void setControlPoints(std::vector<sf::Vector2f> const &pts);

        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);
        virtual void draw(sf::RenderWindow& window) const;

        std::vector<sf::Vector2f> controlPoints;
        std::vector<float> railLengths;
        Train train;
    };
} // namespace loopline

#endif