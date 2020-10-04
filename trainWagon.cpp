#include "trainWagon.hpp"

namespace loopline
{
    TrainWagon::TrainWagon(float length)
        : length(length)
    {
    }

    TrainWagon::~TrainWagon()
    {
    }

    TrainWagon::TrainWagon(TrainWagon const &wagon)
        : length(wagon.length)
    {
        setSprite(*wagon.sprite.getTexture(), wagon.sprite.getTextureRect(), wagon.sprite.getOrigin());
    }

    void TrainWagon::update(sf::Time const &deltaTime)
    {
    }

    void TrainWagon::fixedUpdate(sf::Time const &deltaTime)
    {
    }
} // namespace loopline