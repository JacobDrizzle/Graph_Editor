#include "Graph.h"
#include <algorithm>
#include <cmath>
#include <iostream>

// Constructor: Initializes the graph with the given points and segments.
Graph::Graph(const std::vector<Point>& points, const std::vector<Segment>& segments)
    : points(points), segments(segments), lastPointIndex(-1) {
    if (!points.empty()) {
        lastPointIndex = this->points.size() - 1;
    }
}

// Adds a new point to the graph.
void Graph::addPoint(const Point& point) {
    points.push_back(point);
}

// Checks if a given point already exists in the graph.
bool Graph::containsPoint(const Point& point) const {
    return std::find_if(points.begin(), points.end(), [&point](const Point& p) {
        return p.equals(point);
    }) != points.end();
}

// Tries to add a new point to the graph. Returns true if the point was added.
bool Graph::tryAddPoint(const Point& point) {
    if (!containsPoint(point)) {
        addPoint(point);
        return true;
    }
    return false;
}

// Sets the index of the last point that was added to the graph.
void Graph::setLastPointIndex(int index) {
    lastPointIndex = index;
}

// Gets the last point that was added to the graph.
const Point* Graph::getLastPoint() const {
    if (lastPointIndex >= 0 && lastPointIndex < points.size()) {
        return &points[lastPointIndex];
    }
    return nullptr;
}

// Finds and returns the nearest point in the graph to a specified point.
Point* Graph::findNearestPoint(const Point& newPoint) {
    if (points.empty()) {
        return nullptr;
    }

    Point* nearestPoint = &points[0];
    float minDistance = std::sqrt(std::pow(newPoint.x - nearestPoint->x, 2) + std::pow(newPoint.y - nearestPoint->y, 2));

    for (auto& point : points) {
        float distance = std::sqrt(std::pow(newPoint.x - point.x, 2) + std::pow(newPoint.y - point.y, 2));
        if (distance < minDistance && distance > 0) {
            minDistance = distance;
            nearestPoint = &point;
        }
    }

    return nearestPoint;
}

// Removes a specified point and any segments connected to it from the graph.
void Graph::removePoint(const Point& point) {
    auto it = std::remove_if(segments.begin(), segments.end(), [&point](const Segment& seg) {
        return seg.includes(point);
    });
    segments.erase(it, segments.end());

    points.erase(std::remove_if(points.begin(), points.end(), [&point](const Point& p) {
        return p.equals(point);
    }), points.end());
}

// Adds a new segment to the graph.
void Graph::addSegment(const Segment& seg) {
    segments.push_back(seg);
}

// Checks if a given segment already exists in the graph.
bool Graph::containsSegment(const Segment& seg) const {
    return std::find_if(segments.begin(), segments.end(), [&seg](const Segment& s) {
        return s.equals(seg);
    }) != segments.end();
}

// Tries to add a new segment to the graph. Returns true if the segment was added.
bool Graph::tryAddSegment(const Segment& seg) {
    if (!containsSegment(seg) && !seg.p1.equals(seg.p2)) {
        addSegment(seg);
        return true;
    }
    return false;
}

// Removes a specified segment from the graph.
void Graph::removeSegment(const Segment& seg) {
    segments.erase(std::remove_if(segments.begin(), segments.end(), [&seg](const Segment& s) {
        return s.equals(seg);
    }), segments.end());
}

// Draws all points and segments of the graph on an SFML render window.
void Graph::draw(sf::RenderWindow& window) {
    for (const auto& seg : segments) {
        seg.draw(window);
    }

    for (const auto& point : points) {
        point.draw(window);
    }
}
