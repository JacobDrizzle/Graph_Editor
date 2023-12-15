#include "Point.h"
#include <string>

// Checks if this point is equal to another point.
// Two points are equal if their x and y coordinates are identical.
bool Point::equals(const Point& other) const {
    return x == other.x && y == other.y;
}

// Draws the point on the given render window as a circle.
// The size parameter determines the diameter of the circle.
// The color parameter determines the color of the circle.
void Point::draw(sf::RenderWindow& window, float size, sf::Color color) const {

    // Create a circle with radius equal to half the size
    sf::CircleShape shape(size);
    shape.setFillColor(color);
    shape.setPosition(x - size, y - size);

    // Draw the circle on the window
    window.draw(shape);
}
