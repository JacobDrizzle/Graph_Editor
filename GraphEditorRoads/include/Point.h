#ifndef POINT_H
#define POINT_H

#include <SFML/Graphics.hpp>
#include <string>

// The Point class represents a point in 2D space.
class Point {
public:
    Point(){};

    Point(const Point& original, double angle, double radius, const int id);
    float x, y; // Coordinates of the point
    int id;
    // Constructor: Initializes a new point with the given x and y coordinates.
    Point(float x, float y, const int id) : x(x), y(y), id(id) {};

    Point(float x, float y) : x(x), y(y){};

    // Compares this point to another point for equality.
    // Returns true if both the x and y coordinates are the same.
    bool equals(const Point& other) const;

    // Draws this point on the given SFML render window.
    // The point is drawn as a circle with a specified size and color.
    void draw(sf::RenderWindow& window, float size = 12, sf::Color color = sf::Color(0, 0, 0)) const;
};

#endif // POINT_H
