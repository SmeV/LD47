#include "rails.hpp"

#include <math.h>
#include <cassert>

namespace loopline
{
    Rails::Rails(std::vector<sf::Vector2f> pts)
        : controlPoints(pts)
    {
        railLengths.resize(controlPoints.size());
        auto controlDiff = controlPoints[0] - controlPoints[controlPoints.size() - 1];
        float railLength = 0.f;
        for(unsigned int i=1; i<controlPoints.size(); i++)
        {
            auto controlDiff = controlPoints[i] - controlPoints[i-1];
            railLength += sqrtf(powf(controlDiff.x, 2.0f) + powf(controlDiff.y, 2.0f));
            railLengths[i-1] = railLength;
        }

        controlDiff = controlPoints[0] - controlPoints[controlPoints.size() - 1];
        railLength += sqrtf(powf(controlDiff.x, 2.0f) + powf(controlDiff.y, 2.0f));
        railLengths[controlPoints.size()-1] = railLength;

    }

    Rails::~Rails()
    {
    }

    sf::Vector2f Rails::getWorldPosition(float railPosition) const
    {
        assert(railPosition <= railLengths[railLengths.size()-1]);

        int previousControl = 0;
        int nextControl = 1;
        float railPositionDiscard = 0.f;
        while(railPosition > railLengths[previousControl]) 
        {
            railPositionDiscard = railLengths[previousControl];

            previousControl++; 
            nextControl++;
        }

        if(nextControl == railLengths.size()) nextControl = 0;
        float multiplier = (railPosition - railPositionDiscard) / (railLengths[previousControl] - railPositionDiscard);

        return multiplier * controlPoints[nextControl] + (1.f - multiplier) * controlPoints[previousControl];
    }

    void Rails::update(sf::Time const &deltaTime)
    {
        train.update(deltaTime);
        train.railPosition = fmod(train.railPosition, railLengths[railLengths.size()-1]);

        train.setWorldposition(getWorldPosition(train.railPosition));
    }

    void Rails::fixedUpdate(sf::Time const &deltaTime)
    {
        train.fixedUpdate(deltaTime);
    }

    void Rails::draw(sf::RenderWindow &window) const
    {
        train.draw(window);
    }

} // namespace loopline