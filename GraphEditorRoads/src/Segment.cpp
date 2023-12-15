#include "Segment.h"
#include <cmath>
#include "Constants.h"

// Checks if this segment is equal to another segment.
// Segments are considered equal if they have the same end points, in any order.
bool Segment::equals(const Segment& other) const {
    return (p1.equals(other.p1) && p2.equals(other.p2)) ||
           (p1.equals(other.p2) && p2.equals(other.p1));
}

// Checks if this segment includes a given point.
// A segment includes a point if the point is one of the segment's end points.
bool Segment::includes(const Point& point) const {
    return p1.equals(point) || p2.equals(point);
}

float Segment::length() const {
    return std::sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}

// Draws the segment on the given render window.
// The segment is drawn as a line of specified width and color.
void Segment::draw(sf::RenderWindow& window, float width, sf::Color color) const {
    // Convert end points to SFML vector format
    sf::Vector2f p1Vec(p1.x, p1.y);
    sf::Vector2f p2Vec(p2.x, p2.y);

    // Calculate direction and length of the line
    sf::Vector2f direction = p2Vec - p1Vec;
    float lineLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Create a rectangle shape to represent the line segment
    sf::RectangleShape line(sf::Vector2f(lineLength, width));
    line.setPosition(p1Vec); // Set the position to the first end point
    line.setRotation(atan2(direction.y, direction.x) * 180.0f / M_PI); // Set the rotation to align with the direction
    line.setFillColor(color); // Set the color of the line

    window.draw(line); // Draw the line on the window
}
