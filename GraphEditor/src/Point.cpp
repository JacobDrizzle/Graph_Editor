#include "Point.h"

// Constructor implementation: Initializes the point with given x and y coordinates.
Point::Point(float x, float y) : x(x), y(y) {}

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
    sf::CircleShape circle(size / 2);

    // Position the circle so that its center is at (x, y)
    circle.setPosition(x - size / 2, y - size / 2);

    // Set the fill color of the circle
    circle.setFillColor(color);

    // Draw the circle on the window
    window.draw(circle);
}
