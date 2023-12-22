#ifndef WORLD_H
#define WORLD_H

#include "Graph.h"
#include "GraphEditor.h"

class World {
public:
    World(Graph& graph, GraphEditor& editor);

    void generateLevel();
    void draw();

private:
    Graph& graph;
    GraphEditor& editor;

    // Additional methods for level management
};

#endif // WORLD_H
