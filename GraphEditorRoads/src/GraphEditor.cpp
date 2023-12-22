#include "GraphEditor.h"
#include <cmath>
#include <iostream>
#include "utils.h"
#include "Viewport.h"
#include "Constants.h"
#include "Envelope.h"

// Constructor: Initializes the graph editor with a reference to the SFML window and the graph.
GraphEditor::GraphEditor(sf::RenderWindow& window, Graph& graph, Viewport& viewport)
    : window(window), graph(graph), viewport(viewport), selected(nullptr), hovered(nullptr), dragging(false) {}


// Draws a temporary dashed segment from the selected point to the mouse cursor or nearest point.
void GraphEditor::drawTemporarySegment() {
    if (selected) {
        Point mousePoint = getMousePoint();

        Point* nearest = graph.findNearestPoint(mousePoint);
        sf::Vector2f start(selected->x, selected->y);

        float hoverDistanceThreshold = 25.0f;
        bool isHoveringNearestPoint = nearest && distance(*nearest, mousePoint) < hoverDistanceThreshold;

        sf::Vector2f end = isHoveringNearestPoint ? sf::Vector2f(nearest->x, nearest->y) : sf::Vector2f(mousePoint.x, mousePoint.y);
        drawDashedLine(start, end, sf::Color::Red, 2.5f);
    }
}

// Draws a dashed line between two points.
void GraphEditor::drawDashedLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color, float thickness) {

    const float dashLength = 10.0f;
    const float gapLength = 5.0f;

    sf::Vector2f direction = end - start;
    float lineLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    direction /= lineLength; // Normalize
    sf::Vector2f perpendicular(-direction.y, direction.x); // Perpendicular vector for thickness

    for (float i = 0.0f; i < lineLength; i += dashLength + gapLength) {

        float currentDashLength = std::min(dashLength, lineLength - i);
        sf::RectangleShape dash(sf::Vector2f(currentDashLength, thickness));

        dash.setPosition(start + direction * i + perpendicular * (thickness / 2.0f));
        dash.setRotation(static_cast<float>(atan2(direction.y, direction.x) * 180.0f / M_PI));

        dash.setFillColor(color);
        window.draw(dash);
    }
}

// Draws a temporary point at the current mouse cursor position.
void GraphEditor::drawTemporaryPoint() {

    sf::CircleShape tempPoint(5);
    tempPoint.setFillColor(sf::Color::White);

    //tempPoint.setPosition(static_cast<float>(mouse.x) - 5, static_cast<float>(mouse.y) - 5); // Center the point on the mouse

    sf::Vector2f worldMousePos = viewport.toWorldCoordinates(mouse);
    tempPoint.setPosition(worldMousePos.x - 5, worldMousePos.y - 5);

    window.draw(tempPoint);
}

void GraphEditor::handleShiftClick() {
    Point mousePoint = getMousePoint(); // Convert the mouse coordinates to the graph's coordinate system
    Segment* nearestSegment = graph.findNearestSegment(mousePoint);

    if (nearestSegment) {
        graph.removeSegmentById(nearestSegment->id);
    }
}

// Handles right mouse button down events, deselecting or removing points.
void GraphEditor::handleRightMouseDown(const sf::Event& event) {
    bool ctrlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                       sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

    bool shftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                       sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

    if(shftPressed) {
        Point mousePoint = getMousePoint();
        Segment* tempSeg = graph.findNearestSegment(mousePoint);
        std::cout << "Nearest Segment ID: (" << tempSeg->id << ")\n";
        handleShiftClick();
    }

    if(selected != nullptr){
        selected = nullptr;
    }

    if (ctrlPressed && hovered) {
        if(hovered!= nullptr)
            removePoint(hovered);
    }
}

// Handles left mouse button down events, adding points or segments.
void GraphEditor::handleLeftMouseDown(const sf::Event& event) {

    Point mousePoint = getMousePoint();
    Point* nearest = graph.findNearestPoint(mousePoint);

    float hoverDistanceThreshold = 25.0f;
    bool isHoveringNearestPoint = nearest && distance(*nearest, mousePoint) < hoverDistanceThreshold;

    if (isHoveringNearestPoint) {
        if (selected && nearest != selected) {
            graph.addSegment(Segment(*selected, *nearest));
            //std::cout << "Segment added between selected and nearest point\n";
        }
        selected = nearest;
    } else {
        graph.addPoint(mousePoint);
        Point* newPoint = &graph.points.back();
        if (selected) {
            graph.addSegment(Segment(*selected, *newPoint));
           // std::cout << "Segment added between selected and new point\n";
        }
        selected = newPoint;
    }

    if (selected) {
        std::vector<Point> intersections = graph.findIntersections();
        for (const auto& intersection : intersections) {
            std::cout << "Intersection found at: ("
                      << intersection.x << ", " << intersection.y << ")" << std::endl;
        }
    }
    dragging = true;
    graph.updateBoundary(mousePoint);
}

// Handles different types of events like mouse movement and button presses.
void GraphEditor::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        handleMouseMove(event);
    } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
                if(selected != nullptr)
                    graph.getConnectedSegments(*selected);


            handleRightMouseDown(event);
        } else if (event.mouseButton.button == sf::Mouse::Left){
            handleLeftMouseDown(event);
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        dragging = false;
    }
}

// Handles mouse move events, updating the mouse position and hovered point.
void GraphEditor::handleMouseMove(const sf::Event& event) {
    // Convert event mouse position to your world coordinate or keep it as-is based on how your coordinate system is set up
    mouse = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    sf::Vector2f worldMousePos = viewport.toWorldCoordinates(mouse);

    // If a point is selected and we are dragging it
    if (dragging && selected) {
        // Update the position of the selected point to where the mouse is
        selected->x = worldMousePos.x;
        selected->y = worldMousePos.y;

        // Update the segments connected to this point
        std::vector<int> connectedSegmentIds = graph.getConnectedSegmentIds(*selected);

        for (int id : connectedSegmentIds) {
            // Assuming that segment IDs are stored as integers and are unique
            for (Segment& segment : graph.segments) {
                if (std::stoi(segment.id) == id) {
                    // Check which end of the segment the selected point corresponds to
                    if (segment.p1.id == selected->id) {
                        segment.p1.x = selected->x;
                        segment.p1.y = selected->y;
                    } else if (segment.p2.id == selected->id) {
                        segment.p2.x = selected->x;
                        segment.p2.y = selected->y;
                    }
                }
            }
        }

        // After updating the segments, you should redraw the graph to reflect the changes
        // Depending on how your rendering is set up, you might need to invalidate the current frame
        // and trigger a redraw. This often involves calling a method to clear the screen and then
        // redraw all elements. This might look something like this:
        updateConnectedSegmentsAndEnvelopes(*selected);
        window.clear(); // Clear the window or the drawing surface
        graph.draw(window); // Redraw the entire graph
    }

    // Update the hovered point to be the nearest point to the mouse cursor
    Point tempPoint = getMousePoint();
    hovered = graph.findNearestPoint(tempPoint);
}


// Call this method whenever you move a point to update the connected segments and envelopes
void GraphEditor::updateConnectedSegmentsAndEnvelopes(const Point& selectedPoint) {
    std::vector<int> connectedSegmentIds = graph.getConnectedSegmentIds(selectedPoint);
    for (int id : connectedSegmentIds) {
        for (Segment& segment : graph.segments) {
            if (segment.id == std::to_string(id)) {
                // Update the segment points...
                // ...
                // Find the corresponding Envelope and update it
                Envelope* env = graph.findEnvelope(segment);
                if (env != nullptr) {
                    env->updateSkeleton(segment);
                    // Assuming you have a method to update the visual representation of the envelope:
                    env->updateRoundedRect();
                }
            }
        }
    }
}
// Selects a given point.
void GraphEditor::selectPoint(Point* point) {
    if (selected && point != selected) {
        graph.addSegment(Segment(*selected, *point));
    }
    selected = point;
}

// Removes a given point from the graph.
void GraphEditor::removePoint(Point* point) {
    if(graph.points.size() > 0){
        if (point != nullptr) {
            //std::cout << "Removing point: " << (point ? point->id : -1) << std::endl;
            graph.removePoint(*point);

            if (selected == point) {
                selected = nullptr;
            }
            if (hovered == point) {
                hovered = nullptr;
            }
        }
    }
}

Point GraphEditor::getMousePoint(){
    sf::Vector2f worldMousePos = viewport.toWorldCoordinates(mouse);
    Point mousePoint(worldMousePos.x, worldMousePos.y, (graph.points.size() + 1));
    //std::cout << "getMousePoint ID: " << mousePoint.id << std::endl;
    return mousePoint;
}

// Draws the graph, hovered points, and selected points.
void GraphEditor::draw() {

    graph.draw(window);

    for (const auto& envelope : graph.roadEnvelopes) {
        envelope.draw(window);
    }

    Point mousePoint = getMousePoint();
    Point* nearest = graph.findNearestPoint(mousePoint);
    float hoverDistanceThreshold = 25.0f;
    bool isHoveringNearestPoint = nearest && distance(*nearest, mousePoint) < hoverDistanceThreshold;

    if (hovered && isHoveringNearestPoint) {
        hovered->draw(window, 13, sf::Color::Red);
    }
    if (selected) {
        selected->draw(window, 13, sf::Color::Blue);
    }
}
