#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <memory> // Add this line to include the definition for std::shared_ptr
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class ResourceManager
{
    public:
        std::shared_ptr<sf::Texture> loadTexture(const std::string& name, const std::string& filename) {
            auto texture = std::make_shared<sf::Texture>();
            if (!texture->loadFromFile(filename)) {
                throw std::runtime_error("Failed to load texture: " + filename);
            } else {
                texture->setRepeated(true); // Enable texture repeat mode
            }
            textures[name] = texture;
            return texture;
        }

        std::shared_ptr<sf::Texture> getTexture(const std::string& name) {
            auto found = textures.find(name);
            if (found != textures.end()) {
                return found->second;
            }
            throw std::runtime_error("Texture not found: " + name);
        }
    protected:

    private:
        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;
};

#endif // RESOURCEMANAGER_H
