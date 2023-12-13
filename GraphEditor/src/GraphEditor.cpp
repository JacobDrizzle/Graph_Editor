#include "GraphEditor.h"
#include <cmath>
#include <iostream>
#include "utils.h"
#include "Viewport.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Constructor: Initializes the graph editor with a reference to the SFML window and the graph.
GraphEditor::GraphEditor(sf::RenderWindow& window, Graph& graph, Viewport& viewport)
    : window(window), graph(graph), viewport(viewport), selected(nullptr), hovered(nullptr), dragging(false) {}

// Handles different types of events like mouse movement and button presses.
void GraphEditor::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        handleMouseMove(event);
    } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
            handleRightMouseDown(event);
        } else {
            handleLeftMouseDown(event);
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        dragging = false;
    }
}

// Handles mouse move events, updating the mouse position and hovered point.
void GraphEditor::handleMouseMove(const sf::Event& event) {
    mouse = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    hovered = graph.findNearestPoint(Point(static_cast<float>(mouse.x), static_cast<float>(mouse.y)));

    if (dragging && selected) {
        selected->x = static_cast<float>(mouse.x);
        selected->y = static_cast<float>(mouse.y);
    }
}

// Draws a temporary dashed segment from the selected point to the mouse cursor or nearest point.
void GraphEditor::drawTemporarySegment() {
    if (selected) {
          sf::Vector2f worldMousePos = viewport.toWorldCoordinates(mouse);
        Point mousePoint(worldMousePos.x, worldMousePos.y);
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
    tempPoint.setFillColor(sf::Color::Blue);
    //tempPoint.setPosition(static_cast<float>(mouse.x) - 5, static_cast<float>(mouse.y) - 5); // Center the point on the mouse
    sf::Vector2f worldMousePos = viewport.toWorldCoordinates(mouse);
    tempPoint.setPosition(worldMousePos.x - 5, worldMousePos.y - 5);
    window.draw(tempPoint);
}

// Handles right mouse button down events, deselecting or removing points.
void GraphEditor::handleRightMouseDown(const sf::Event& event) {
    if (selected) {
        selected = nullptr;
    } else if (hovered) {
        removePoint(hovered);
    }
}

// Handles left mouse button down events, adding points or segments.
void GraphEditor::handleLeftMouseDown(const sf::Event& event) {

    sf::Vector2f worldMousePos = viewport.toWorldCoordinates(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    Point mousePoint(worldMousePos.x, worldMousePos.y);

    Point* nearest = graph.findNearestPoint(mousePoint);
    float hoverDistanceThreshold = 25.0f;
    bool isHoveringNearestPoint = nearest && distance(*nearest, mousePoint) < hoverDistanceThreshold;
    if (isHoveringNearestPoint) {
        if (selected && nearest != selected) {
            graph.addSegment(Segment(*selected, *nearest));
            std::cout << "Segment added between selected and nearest point\n";
        }
        selected = nearest;
    } else {
        graph.addPoint(mousePoint);
        Point* newPoint = &graph.points.back();
        std::cout << "New Point: (" << newPoint->x << ", " << newPoint->y << ")\n";
        if (selected) {
            graph.addSegment(Segment(*selected, *newPoint));
            std::cout << "Segment added between selected and new point\n";
        }
        selected = newPoint;
    }
    dragging = true;
    graph.updateBoundary(mousePoint);
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
    graph.removePoint(*point);
    hovered = nullptr;
    if (selected == point) {
        selected = nullptr;
    }
}

// Draws the graph, hovered points, and selected points.
void GraphEditor::draw() {
    graph.draw(window);
    if (hovered) {
        hovered->draw(window, 20, sf::Color::Red);
    }
    if (selected) {
        selected->draw(window, 20, sf::Color::Blue);
    }
}
