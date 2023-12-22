#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "GraphEditor.h"
#include "Viewport.h"
#include "Button.h"
#include "World.h"

class Application {
public:
    Application();
    void run();
    void update();
    void render();
    bool isRunning() const;

private:
    sf::RenderWindow window;
    Viewport viewport;
    Graph graph;
    GraphEditor editor;
    World world;
    Button saveButton;
    Button resetButton;

    void initialize();
    void handleEvents();
};

#endif // APPLICATION_H
