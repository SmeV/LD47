#ifndef __LOOPLINE_TEXTUREMANAGER_HPP__
#define __LOOPLINE_TEXTUREMANAGER_HPP__

#include <unordered_map>
#include <string>
#include <memory>

#include "SFML/Graphics.hpp"

namespace loopline
{
    class TextureManager
    {
    public:
        TextureManager();

        sf::Texture const& getTexture(std::string const &name) const;

        sf::Texture const& loadTexture(std::string const &filename, std::string const &name);

    private:

        std::unordered_map<std::string, sf::Texture> textures;
    };
}; // namespace loopline

#endif