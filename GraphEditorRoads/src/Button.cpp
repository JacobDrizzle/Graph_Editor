#include "Button.h"

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(label);
}

bool Button::isMouseOver(sf::RenderWindow& window) {
    sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(window);
    return shape.getGlobalBounds().contains(mousePos);
}

void Button::checkClick(sf::RenderWindow& window) {
    if (isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        onClick();
    }
}
