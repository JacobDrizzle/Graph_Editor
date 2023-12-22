#include "World.h"

World::World(Graph& graph, GraphEditor& editor) : graph(graph), editor(editor) {
    generateLevel();
}

void World::generateLevel() {
    // Initialize or generate the level using the Graph class
    // This can include setting up initial points, segments, etc.
}

void World::draw() {
    // Draw a temporary point at the current mouse position
    editor.drawTemporaryPoint();

    // Draw a temporary segment from the selected point to the mouse position or nearest point
    editor.drawTemporarySegment();

    // Draw the level elements. The Graph class handles the drawing of envelopes.
    editor.draw();
    // Add any additional drawing or level-specific elements here
}
