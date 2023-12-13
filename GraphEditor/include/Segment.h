#ifndef SEGMENT_H
#define SEGMENT_H

#include "Point.h"

// The Segment class represents a line segment defined by two points in 2D space.
class Segment {
public:
    // Endpoints of the segment
    Point p1, p2;

    // Constructor: Initializes a new segment with the given end points.
    Segment(const Point& p1, const Point& p2);

    // Checks if this segment is equal to another segment.
    // Two segments are equal if they have the same end points.
    bool equals(const Segment& other) const;

    // Checks if this segment includes a given point.
    // A point is included in a segment if it is one of the segment's end points.
    bool includes(const Point& point) const;

    // Draws this segment on the given SFML render window.
    // The segment is drawn as a line with a specified width and color.
    void draw(sf::RenderWindow& window, float width = 2, sf::Color color = sf::Color::Black) const;
};

#endif // SEGMENT_H
