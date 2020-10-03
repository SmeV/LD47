#include "textureManager.hpp"

namespace loopline
{
    TextureManager::TextureManager()
    {
        textures["NULL"] = sf::Texture{};
    }

    sf::Texture const& TextureManager::getTexture(std::string const &name) const
    {
        auto it = textures.find(name);
        if(it != textures.end()) return it->second;

        return textures.find("NULL")->second;
    }

    sf::Texture const& TextureManager::loadTexture(std::string const &filename, std::string const &name)
    {
        if(textures.find(name) != textures.end()) return textures[name];

        textures[name].loadFromFile(filename);

        return textures[name];
    }
} // namespace loopline