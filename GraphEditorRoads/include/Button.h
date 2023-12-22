#ifndef BUTTON_H
#define BUTTON_H


#include <SFML/Graphics.hpp>
#include <functional>

class Button {
public:
    Button(const sf::Vector2f& position, const sf::Vector2f& size,
           const std::string& text, std::function<void()> onClick)
        : onClick(onClick) {

        shape.setSize(size);
        shape.setPosition(position);
        shape.setFillColor(sf::Color(0,0,0,120));

        font.loadFromFile("res/font.ttf"); // Load your font file
        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(20);
        label.setFillColor(sf::Color::White);
        label.setPosition(position.x + 15, position.y + 10);
    }

    void draw(sf::RenderWindow& window);

    bool isMouseOver(sf::RenderWindow& window);

    void checkClick(sf::RenderWindow& window);

private:
    sf::RectangleShape shape;
    sf::Text label;
    sf::Font font;
    std::function<void()> onClick;
};
#endif // BUTTON_H
