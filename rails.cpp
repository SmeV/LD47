#include "rails.hpp"
#include "helper.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

namespace loopline
{
    Rails::Rails(std::vector<sf::Vector2f> const &pts, Pathfinding path)
        : path(path)
    {
        setControlPoints(pts);
    }

    Rails::~Rails()
    {
    }

    void Rails::setControlPoints(std::vector<sf::Vector2f> const &pts)
    {
        controlPoints = pts;
        railLengths.empty();

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

    sf::Vector2f Rails::getWorldPosition(float railPosition) const
    {
        sf::Vector2f worldPos;
        railPosition = fmod(railPosition, railLengths[railLengths.size() - 1]);
        int previouspreviousControl = railLengths.size() - 1;
        int previousControl = 0;
        int nextControl = 1;
        int nextnextControl = 2;
        float railPositionDiscard = 0.f;
        while (railPosition > railLengths[previousControl])
        {
            railPositionDiscard = railLengths[previousControl];

            previousControl = (previousControl + 1) % railLengths.size();
            nextControl = (nextControl + 1) % railLengths.size();
            previouspreviousControl = (previouspreviousControl + 1) % railLengths.size();
            nextnextControl = (nextnextControl + 1) % railLengths.size();
        }

        float multiplier = (railPosition - railPositionDiscard) / (railLengths[previousControl] - railPositionDiscard);
        switch (path)
        {
        case CUBIC:
            worldPos = CubicInterpolate(controlPoints[previouspreviousControl], controlPoints[previousControl], controlPoints[nextControl], controlPoints[nextnextControl], multiplier);
            break;
        case LINEAR:
            worldPos = multiplier * controlPoints[nextControl] + (1.f - multiplier) * controlPoints[previousControl];
            break;
        }

        return worldPos;
    }

    void Rails::update(sf::Time const &deltaTime)
    {
        for(auto& train : trains)
        {
            train.update(deltaTime);
            train.railPosition = fmod(train.railPosition, railLengths[railLengths.size() - 1]);

            auto firstTrainPoint = getWorldPosition(train.railPosition);
            auto secondTrainPoint = getWorldPosition(train.railPosition - train.length);
            auto diff = firstTrainPoint - secondTrainPoint;
            float length = sqrtf(powf(diff.x, 2.0f) + powf(diff.y, 2.0f));
            diff /= length;
            float rotationAngle = atan2f(diff.y, diff.x);

            float rotationAngleDeg = rotationAngle / M_PI * 180.f;

            if (rotationAngleDeg > 90.f || rotationAngleDeg < -90.f)
                train.setSpriteScale({1.f, -1.f});
            else
                train.setSpriteScale({1.f, 1.f});

            train.setSpritePosition(firstTrainPoint);
            train.setSpriteRotation(rotationAngleDeg);
        }
    }

    void Rails::fixedUpdate(sf::Time const &deltaTime)
    {
        for(auto& train : trains)
        {
            train.fixedUpdate(deltaTime);
        }
    }

    void Rails::draw(sf::RenderWindow &window) const
    {
        for(auto& train : trains)
        {
            train.draw(window);
        }
    }

} // namespace loopline