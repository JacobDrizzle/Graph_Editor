#include <SFML/Graphics.hpp>
#include "Point.h"
#include "Segment.h"
#include "Graph.h"
#include "GraphEditor.h"
#include "Viewport.h"
#include "Polygon.h"
#include "GeometryUtils.h"

int main() {
    // Create an SFML window with a size of 800x800 pixels and title "Spatial Graphs"
    sf::RenderWindow window(sf::VideoMode(800, 800), "Spatial Graphs");
    Viewport viewport(window);

    // Create a graph with the initialized points and segments
    Graph graph({}, {});

    // Initialize the GraphEditor with the SFML window and the created graph
    GraphEditor editor(window, graph, viewport);

    // Main loop that runs as long as the window is open
    while (window.isOpen()) {
        sf::Event event;
        // Poll for events from the window

        while (window.pollEvent(event)) {
            // If the close button is pressed, close the window
            if (event.type == sf::Event::Closed)
                window.close();
            // Pass all events to the GraphEditor for handling
            editor.handleEvent(event);
            viewport.handleEvent(event);
        }
        viewport.update();  // Update the viewport

        // Clear the window with a dark gray color
        window.clear(sf::Color(0, 163, 108));

        // Draw the regular content of the GraphEditor
        editor.draw();

        // Draw a temporary point at the current mouse position
        editor.drawTemporaryPoint();

        // Draw a temporary segment from the selected point to the mouse position or nearest point
        editor.drawTemporarySegment();

        // Display the drawn content on the window
        window.display();
    }

    return 0;
}
