#include "inputManager.hpp"

namespace loopline
{

    InputManager::InputManager()
    {
    }

    void InputManager::eventKey(sf::Keyboard::Key const &key) const
    {
        auto it = eventKeyBindings.find(key);

        if (it == eventKeyBindings.end())
            return;

        it->second->execute();
    }

    void InputManager::eventKeyReleased(sf::Keyboard::Key const &key) const
    {
        auto it = eventKeyReleaseBindings.find(key);

        if (it == eventKeyReleaseBindings.end())
            return;

        it->second->execute();
    }

    void InputManager::update(sf::Time const &deltaTime) const
    {
        for (auto &it : updateKeyBindings)
        {
            if (sf::Keyboard::isKeyPressed(it.first))
            {
                it.second->execute();
            }
        }
    }

    void InputManager::fixedUpdate(sf::Time const &deltaTime) const
    {
        for (auto &it : fixedUpdateKeyBindings)
        {
            if (sf::Keyboard::isKeyPressed(it.first))
            {
                it.second->execute();
            }
        }
    }

    bool InputManager::addEventCommand(std::shared_ptr<Command> newCommand, sf::Keyboard::Key const &key)
    {
        auto it = eventKeyBindings.find(key);

        if (it != eventKeyBindings.end())
            return false;

        eventKeyBindings[key] = newCommand;
        return true;
    }

    bool InputManager::deleteEventCommand(std::shared_ptr<Command> command)
    {
        for (auto it = eventKeyBindings.begin(); it != eventKeyBindings.end(); it++)
        {
            if (it->second == command)
            {
                eventKeyBindings.erase(it);
                return true;
            }
        }
        return false;
    }

    bool InputManager::unbindEventKey(sf::Keyboard::Key const &key)
    {
        auto it = eventKeyBindings.find(key);

        if (it == eventKeyBindings.end())
            return false;

        eventKeyBindings.erase(it);
        return true;
    }

    bool InputManager::addReleaseEventCommand(std::shared_ptr<Command> newCommand, sf::Keyboard::Key const &key)
    {
        auto it = eventKeyReleaseBindings.find(key);

        if (it != eventKeyReleaseBindings.end())
            return false;

        eventKeyReleaseBindings[key] = newCommand;
        return true;
    }

    bool InputManager::deleteReleaseEventCommand(std::shared_ptr<Command> command)
    {
        for (auto it = eventKeyReleaseBindings.begin(); it != eventKeyReleaseBindings.end(); it++)
        {
            if (it->second == command)
            {
                eventKeyReleaseBindings.erase(it);
                return true;
            }
        }
        return false;
    }

    bool InputManager::unbindReleaseEventKey(sf::Keyboard::Key const &key)
    {
        auto it = eventKeyReleaseBindings.find(key);

        if (it == eventKeyReleaseBindings.end())
            return false;

        eventKeyReleaseBindings.erase(it);
        return true;
    }

    bool InputManager::addUpdateCommand(std::shared_ptr<Command> newCommand, sf::Keyboard::Key const &key)
    {
        auto it = updateKeyBindings.find(key);

        if (it != updateKeyBindings.end())
            return false;

        updateKeyBindings[key] = newCommand;
        return true;
    }

    bool InputManager::deleteUpdateCommand(std::shared_ptr<Command> command)
    {
        for (auto it = updateKeyBindings.begin(); it != updateKeyBindings.end(); it++)
        {
            if (it->second == command)
            {
                updateKeyBindings.erase(it);
                return true;
            }
        }
        return false;
    }

    bool InputManager::unbindUpdateKey(sf::Keyboard::Key const &key)
    {
        auto it = updateKeyBindings.find(key);

        if (it == updateKeyBindings.end())
            return false;

        updateKeyBindings.erase(it);
        return true;
    }

    bool InputManager::addFixedUpdateCommand(std::shared_ptr<Command> newCommand, sf::Keyboard::Key const &key)
    {
        auto it = fixedUpdateKeyBindings.find(key);

        if (it != fixedUpdateKeyBindings.end())
            return false;

        fixedUpdateKeyBindings[key] = newCommand;
        return true;
    }

    bool InputManager::deleteFixedUpdateCommand(std::shared_ptr<Command> command)
    {
        for (auto it = fixedUpdateKeyBindings.begin(); it != fixedUpdateKeyBindings.end(); it++)
        {
            if (it->second == command)
            {
                fixedUpdateKeyBindings.erase(it);
                return true;
            }
        }
        return false;
    }

    bool InputManager::unbindFixedUpdateKey(sf::Keyboard::Key const &key)
    {
        auto it = fixedUpdateKeyBindings.find(key);

        if (it == fixedUpdateKeyBindings.end())
            return false;

        fixedUpdateKeyBindings.erase(it);
        return true;
    }

}; // namespace loopline