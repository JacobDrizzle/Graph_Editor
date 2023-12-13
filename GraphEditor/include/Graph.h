#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Segment.h"

// The Graph class represents a collection of points and segments in 2D space.
class Graph {
public:
    // Collections of segments and points in the graph
    std::vector<Segment> segments;
    std::vector<Point> points;
    float minX, maxX, minY, maxY;
    // Index of the last point added to the graph
    size_t lastPointIndex;

    // Constructor: Initializes a new graph with optional predefined points and segments.
    Graph(const std::vector<Point>& points = {},
      const std::vector<Segment>& segments = {},
      float minX = std::numeric_limits<float>::max(),
      float maxX = std::numeric_limits<float>::lowest(),
      float minY = std::numeric_limits<float>::max(),
      float maxY = std::numeric_limits<float>::lowest());

    // Adds a point to the graph
    void addPoint(const Point& point);

    // Adds a segment to the graph
    void addSegment(const Segment& seg);

    // Tries to add a point to the graph, returns true if added successfully
    bool tryAddPoint(const Point& point);

    // Tries to add a segment to the graph, returns true if added successfully
    bool tryAddSegment(const Segment& seg);

    // Removes a point from the graph
    void removePoint(const Point& point);

    // Removes a segment from the graph
    void removeSegment(const Segment& seg);

    // Checks if the graph contains a specific point
    bool containsPoint(const Point& point) const;

    // Checks if the graph contains a specific segment
    bool containsSegment(const Segment& seg) const;

    // Finds the nearest point in the graph to a given point
    Point* findNearestPoint(const Point& newPoint);

    // Sets the index of the last point added to the graph
    void setLastPointIndex(int index);

    // Gets the last point added to the graph
    const Point* getLastPoint() const;

    void updateBoundary(const Point& newPoint);

    // Draws the graph on an SFML render window
    void draw(sf::RenderWindow& window);
};

#endif // GRAPH_H
