#ifndef VIEWPORT_H
#define VIEWPORT_H
#include <SFML/Graphics.hpp>

class Viewport {
public:
    Viewport(sf::RenderWindow& window);
    sf::Vector2f toWorldCoordinates(sf::Vector2i screenCoordinates);

    void handleEvent(const sf::Event& event);
    void update();

private:
    void zoom(float factor);
    void pan(sf::Vector2f movement);
    sf::Vector2i toScreenCoordinates(sf::Vector2f worldCoordinates);

    sf::RenderWindow& window;
    sf::View view;
    sf::Vector2f lastMousePos;
    bool isPanning;
};

#endif // VIEWPORT_H
