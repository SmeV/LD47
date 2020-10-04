#ifndef __LOOPLINE_TRAIN_HPP__
#define __LOOPLINE_TRAIN_HPP__

#include <SFML/Graphics.hpp>

#include "command.hpp"
#include "updatable.hpp"
#include "drawable.hpp"

namespace loopline
{
    class Train : public Updatable, public Drawable
    {
    private:
        friend class Rails;
        float acceleration;
        float length;

    public:
        class AccelCommand : public Command
        {
            private:
            Train* train;
            float accel;

            public:
            AccelCommand(Train *train = NULL, float accel = 0.f);
            virtual ~AccelCommand() override;
            virtual void execute() override;
            virtual void undo() override;
        };
        enum TrainState{DRIVING, STOPPED};

        Train();
        ~Train();
        Train(Train const &train);

        virtual void update(sf::Time const &deltaTime);
        virtual void fixedUpdate(sf::Time const &deltaTime);

        virtual void drawUI(sf::RenderWindow &window, sf::Font const &font);

        std::shared_ptr<AccelCommand> accel, deaccel, chime, noaccel;
        float railPosition;
        float speed;
        float maxSpeed = 500.0f;

        int passengers = 0;
        int maxCapacity = 20;
        TrainState state = STOPPED;
    };
} // namespace loopline

#endif