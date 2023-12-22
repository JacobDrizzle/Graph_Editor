#include "Graph.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "RoundedRectangleShape.h"
#include "utils.h"

// Constructor: Initializes the graph with the given points and segments.
Graph::Graph(const std::vector<Point>& points, const std::vector<Segment>& segments,
             float min_x, float max_x, float min_y, float max_y)
    : points(points), segments(segments),
      minX(min_x), maxX(max_x), minY(min_y), maxY(max_y), lastPointIndex(-1) {
    resourceManager.loadTexture("roadTexture", "Assets/road.png");
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

// Checks if a given point already exists in the graph.
bool Graph::containsPoint(const Point& point) const {
    return std::find_if(points.begin(), points.end(), [&point](const Point& p) {
        return p.equals(point);
    }) != points.end();
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

// Tries to add a new point to the graph. Returns true if the point was added.
bool Graph::tryAddPoint(const Point& point) {
    if (!containsPoint(point)) {
        std::cout << "Point Added (" << point.x << "," << point.y << ")" << std::endl;
        addPoint(point);
        return true;
    }
    return false;
}

// Removes a specified point and any segments connected to it from the graph.
void Graph::removePoint(const Point& point) {
    auto it = std::remove_if(segments.begin(), segments.end(), [&point](const Segment& seg) {
        return seg.includes(point);
    });
    if(segments.size() > 0){
    segments.erase(it, segments.end());
    roadEnvelopes.pop_back();
    }
    points.erase(std::remove_if(points.begin(), points.end(), [&point](const Point& p) {
        return p.equals(point);
    }), points.end());
}

// Adds a new segment to the graph.
void Graph::addSegment(const Segment& seg) {
    Segment newSegment(seg.p1, seg.p2, std::to_string(segments.size() + 1));

    std::cout << "Segment added ID: " << newSegment.id << std::endl;
    segments.push_back(newSegment);

    roadEnvelopes.push_back(createRoadEnvelope(newSegment, 25.0));
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

void Graph::removeSegmentById(const std::string& segmentId) {
    auto it = std::remove_if(segments.begin(), segments.end(),
                            [&segmentId](const Segment& seg) { return seg.id == segmentId; });
    if (it != segments.end()) {
        segments.erase(it, segments.end());
    }
     auto envelopeIt = std::remove_if(roadEnvelopes.begin(), roadEnvelopes.end(),
                                         [&segmentId](const Envelope& env) { return env.getSkeleton().id == segmentId; });
        if (envelopeIt != roadEnvelopes.end()) {
            roadEnvelopes.erase(envelopeIt, roadEnvelopes.end());
        }

    std::cout << "Segments Array Size: " << segments.size() << std::endl;
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

Envelope* Graph::findEnvelope(const Segment& segment) {
    for (Envelope& envelope : roadEnvelopes) {
        if (envelope.getSkeleton().id == segment.id) {
            return &envelope;
        }
    }
    return nullptr; // If no matching envelope is found
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
    auto roadTexture = resourceManager.getTexture("roadTexture");

    // Create the Envelope with the segment and width
    Envelope envelope(segment, width);

    // Set the texture for the envelope
    envelope.setTexture(roadTexture);

    return envelope;
}

std::vector<Point> Graph::findIntersections() {
    std::vector<Point> intersections;
    for (size_t i = 0; i < segments.size(); ++i) {
        for (size_t j = i + 1; j < segments.size(); ++j) {
            Point intersection = getIntersection(segments[i].p1, segments[i].p2, segments[j].p1, segments[j].p2);
            if (intersection.x != -1 && intersection.y != -1) {
                intersections.push_back(intersection);
            }
        }
    }
    return intersections;
}
float Graph::calculateDistanceFromPointToSegment(const Point& point, const Segment& segment) {
    float x = point.x;
    float y = point.y;
    float x1 = segment.p1.x;
    float y1 = segment.p1.y;
    float x2 = segment.p2.x;
    float y2 = segment.p2.y;

    // Calculate the differences
    float dx = x2 - x1;
    float dy = y2 - y1;

    // Calculate the t that minimizes the distance.
    float t = ((x - x1) * dx + (y - y1) * dy) / (dx * dx + dy * dy);

    // See if this represents one of the segment's end points or a point in the middle.
    t = std::max(0.0f, std::min(1.0f, t));

    float nearestX = x1 + t * dx;
    float nearestY = y1 + t * dy;

    // Calculate the distance to the nearest point
    return std::sqrt((x - nearestX) * (x - nearestX) + (y - nearestY) * (y - nearestY));
}

Segment* Graph::findNearestSegment(const Point& point) {
    Segment* nearestSegment = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (auto& segment : segments) {
        float distance = calculateDistanceFromPointToSegment(point, segment);
        if (distance < minDistance) {
            minDistance = distance;
            nearestSegment = &segment;
        }
    }
    return nearestSegment;
}

// Checks if a given segment already exists in the graph.
bool Graph::containsSegment(const Segment& seg) const {
    return std::find_if(segments.begin(), segments.end(), [&seg](const Segment& s) {
        return s.equals(seg);
    }) != segments.end();
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
