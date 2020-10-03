#ifndef __LOOPLINE_INPUTMANAGER_HPP__
#define __LOOPLINE_INPUTMANAGER_HPP__

#include <unordered_map>
#include <string>
#include <memory>

#include "SFML/Window.hpp"

#include "command.hpp"
#include "updatable.hpp"

namespace loopline
{
    class InputManager : public Updatable
    {
    public:
        InputManager();

        void eventKey(sf::Keyboard::Key const &key) const;
        void eventKeyReleased(sf::Keyboard::Key const &key) const;
        virtual void update(sf::Time const &deltaTime) const;
        virtual void fixedUpdate(sf::Time const &deltaTime) const;

        bool addEventCommand(std::shared_ptr<Command> newCommand, sf::Keyboard::Key const &key);
        bool deleteEventCommand(std::shared_ptr<Command> command);
        bool unbindEventKey(sf::Keyboard::Key const &key);

        bool addReleaseEventCommand(std::shared_ptr<Command> newCommand, sf::Keyboard::Key const &key);
        bool deleteReleaseEventCommand(std::shared_ptr<Command> command);
        bool unbindReleaseEventKey(sf::Keyboard::Key const &key);

        bool addUpdateCommand(std::shared_ptr<Command> newCommand, sf::Keyboard::Key const &key);
        bool deleteUpdateCommand(std::shared_ptr<Command> command);
        bool unbindUpdateKey(sf::Keyboard::Key const &key);

        bool addFixedUpdateCommand(std::shared_ptr<Command> newCommand, sf::Keyboard::Key const &key);
        bool deleteFixedUpdateCommand(std::shared_ptr<Command> command);
        bool unbindFixedUpdateKey(sf::Keyboard::Key const &key);

    private:

        std::unordered_map<sf::Keyboard::Key, std::shared_ptr<Command>> eventKeyBindings;
        std::unordered_map<sf::Keyboard::Key, std::shared_ptr<Command>> eventKeyReleaseBindings;
        std::unordered_map<sf::Keyboard::Key, std::shared_ptr<Command>> updateKeyBindings;
        std::unordered_map<sf::Keyboard::Key, std::shared_ptr<Command>> fixedUpdateKeyBindings;

    };
}; // namespace loopline

#endif