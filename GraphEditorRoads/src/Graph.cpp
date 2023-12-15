#include "Graph.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "RoundedRectangleShape.h"

// Constructor: Initializes the graph with the given points and segments.
Graph::Graph(const std::vector<Point>& points, const std::vector<Segment>& segments,
             float min_x, float max_x, float min_y, float max_y)
    : points(points), segments(segments),
      minX(min_x), maxX(max_x), minY(min_y), maxY(max_y), lastPointIndex(-1) {
    if (!points.empty()) {
        lastPointIndex = this->points.size() - 1;
        // Initialize boundary based on existing points
        for (const auto& point : this->points) {
            updateBoundary(point);
        }
    }
}

void Graph::updateBoundary(const Point& newPoint) {
    // Update minX and maxX
    if (newPoint.x < minX) minX = newPoint.x;
    if (newPoint.x > maxX) maxX = newPoint.x;

    // Update minY and maxY
    if (newPoint.y < minY) minY = newPoint.y;
    if (newPoint.y > maxY) maxY = newPoint.y;
}

// Adds a new point to the graph.
void Graph::addPoint(const Point& point) {
    if (!containsPoint(point)) {
        Point newPoint(point.x, point.y, points.size() + 1);
        std::cout << "Point ID: " << newPoint.id << std::endl;
        points.push_back(newPoint);
    } else {
        std::cerr << "Point already exists: " << point.x << ", " << point.y << std::endl;
    }
}

// Adds a new segment to the graph.
void Graph::addSegment(const Segment& seg) {
    static int segmentCount = 0;
    Segment newSegment(seg.p1, seg.p2, std::to_string(segments.size() + 1));

    std::cout << "Segment added ID: " << newSegment.id << std::endl;
    segments.push_back(newSegment);

    roadEnvelopes.push_back(createRoadEnvelope(newSegment, 25.0));
}
// Checks if a given point already exists in the graph.
bool Graph::containsPoint(const Point& point) const {
    return std::find_if(points.begin(), points.end(), [&point](const Point& p) {
        return p.equals(point);
    }) != points.end();
}

std::vector<int> Graph::getConnectedSegmentIds(const Point& point) {
    std::vector<int> connectedIds;
    for (const auto& segment : segments) {
        if (segment.p1.id == point.id || segment.p2.id == point.id) {
            connectedIds.push_back(stoi(segment.id));
        }
    }
    //std::cout << "Connected Segments: " << connectedIds.size() << std::endl;
    return connectedIds;
}

std::vector<Segment>& Graph::getConnectedSegments(const Point& point) {
    std::vector<Segment> connectedSegments;
    for (const auto& segment : segments) {
        if (segment.p1.id == point.id || segment.p2.id == point.id) {
            connectedSegments.push_back(segment);
        }
    }
    //std::cout << "Connected Segments: " << connectedSegments.size() << std::endl;
    return connectedSegments;
}


// Tries to add a new point to the graph. Returns true if the point was added.
bool Graph::tryAddPoint(const Point& point) {
    if (!containsPoint(point)) {
        std::cout << "Point Added (" << point.x << "," << point.y << ")" << std::endl;
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

Envelope* Graph::findEnvelope(const Segment& segment) {
    for (Envelope& envelope : roadEnvelopes) {
        if (envelope.getSkeleton().id == segment.id) {
            return &envelope;
        }
    }
    return nullptr; // If no matching envelope is found
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
    roadEnvelopes.pop_back();
    points.erase(std::remove_if(points.begin(), points.end(), [&point](const Point& p) {
        return p.equals(point);
    }), points.end());
}

void Graph::updateEnvelope(Segment& segment) {
    for (auto& envelope : roadEnvelopes) {
        // Match the envelope with the segment using ID or pointers
        if (envelope.getSkeleton().id == segment.id) {
            // Update the skeleton of the envelope
            envelope.updateSkeleton(segment);
            break;
        }
    }
}

Envelope Graph::createRoadEnvelope(const Segment& segment, double width) {
    // Assuming width is the desired road width
    return Envelope(segment, width);
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
    for (size_t i = 0; i < segments.size(); ++i) {
        if (segments[i].equals(seg)) {
            segments.erase(segments.begin() + i);
            roadEnvelopes.erase(roadEnvelopes.begin() + i);
            break;  // Assuming each segment is unique
        }
    }
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
