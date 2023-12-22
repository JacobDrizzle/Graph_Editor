#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "Viewport.h"
#include <stack>
#include <vector>
#include "Envelope.h"

// The GraphEditor class manages the interaction and visualization of a Graph object.
class GraphEditor {
public:
    // Constructor: Initializes the GraphEditor with a reference to the SFML window and the graph.
    GraphEditor(sf::RenderWindow& window, Graph& graph, Viewport& viewport);

    // Handles various events such as mouse movements and button presses.
    void handleEvent(const sf::Event& event);
    void handleShiftClick();
    Point getMousePoint();
    // Update method for handling non-event based logic (if any).
    void update();

    // Draws the graph and any additional editor-specific visuals.
    void draw();

    // Draws a temporary segment from the selected point to the mouse cursor or nearest point.
    void drawTemporarySegment();

    // Draws a temporary point at the current mouse cursor position.
    void drawTemporaryPoint();
    Viewport& viewport;

private:
    // Reference to the SFML render window.
    sf::RenderWindow& window;

    // Current mouse position.
    sf::Vector2i mouse;

    // Reference to the graph being edited.
    Graph& graph;


    // Pointers to the currently selected and hovered points.
    Point* selected;
    Point* hovered;

    // Flag to indicate if a point is being dragged.
    bool dragging;

    // Handles mouse move events.
    void handleMouseMove(const sf::Event& event);

    // Handles left mouse button down events.
    void handleLeftMouseDown(const sf::Event& event);

    // Handles right mouse button down events.
    void handleRightMouseDown(const sf::Event& event);

    // Selects a given point.
    void selectPoint(Point* point);

    // Removes a given point from the graph.
    void removePoint(Point* point);

    void updateConnectedSegmentsAndEnvelopes(const Point& selectedPoint);

    // Draws a dashed line between two points.
    void drawDashedLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color, float thickness);
};

#endif // GRAPHEDITOR_H
