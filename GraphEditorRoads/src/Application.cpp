#include "Application.h"

Application::Application()
    : window(sf::VideoMode(1000, 1000), "Spatial Graphs"),
      viewport(window),
      graph({}, {}),
      editor(window, graph, viewport),
      world(graph, editor),
      saveButton({800, 50}, {100, 50}, "Save", [this](){ /* Save logic here */ }),
      resetButton({800, 110}, {100, 50}, "Reset", [this](){ this->graph = Graph({}, {}); }) {
    initialize();
}

void Application::initialize() {
    bool isMouseOverButton = false;

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

        saveButton.checkClick(window);
        resetButton.checkClick(window);

        // Clear the window with a dark gray color
        window.clear(sf::Color(0, 163, 108));

        // Draw the regular content of the GraphEditor
        world.draw();
        viewport.update();  // Update the viewport
        // Draw your world, graph, etc. with the transformed view
        // world.draw(window); // Assuming this is where you draw the world

        // Save the current view
        sf::View currentView = window.getView();

        // Reset to the default view before drawing your UI
        window.setView(window.getDefaultView());

        // Draw UI elements like buttons which should be unaffected by the view transformations
        saveButton.draw(window);
        resetButton.draw(window);

        // Restore the previous view to continue drawing the rest of the scene
        window.setView(currentView);
        // Display the drawn content on the window
        window.display();
    }
}

void Application::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        editor.handleEvent(event);
        viewport.handleEvent(event);
    }
    saveButton.checkClick(window);
    resetButton.checkClick(window);
}

void Application::update() {
    // Update application state
}

void Application::render() {
    window.clear(sf::Color(0, 163, 108));
    world.draw();
    viewport.update();

    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());

    saveButton.draw(window);
    resetButton.draw(window);

    window.setView(currentView);
    window.display();
}

void Application::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

bool Application::isRunning() const {
    return window.isOpen();
}
