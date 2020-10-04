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
        enum Pathfinding{LINEAR, CUBIC};
        Rails(std::vector<sf::Vector2f> const &pts = {}, Pathfinding path = CUBIC);
        ~Rails();

        sf::Vector2f getWorldPosition(float railPosition) const;

        void setControlPoints(std::vector<sf::Vector2f> const &pts);

        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);
        virtual void draw(sf::RenderWindow& window) const;

        Pathfinding path;
        std::vector<sf::Vector2f> controlPoints;
        std::vector<float> railLengths;
        std::vector<Train> trains;
    };
} // namespace loopline

#endif