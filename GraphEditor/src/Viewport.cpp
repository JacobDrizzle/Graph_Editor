#include "Viewport.h"

Viewport::Viewport(sf::RenderWindow& window)
    : window(window), view(window.getDefaultView()), isPanning(false) {}

void Viewport::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
        zoom(zoomFactor);
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Middle) {
            isPanning = true;
            lastMousePos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Middle) {
            isPanning = false;
        }
    }

    if (event.type == sf::Event::MouseMoved && isPanning) {
        sf::Vector2f newMousePos = window.mapPixelToCoords(
            sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        sf::Vector2f delta = lastMousePos - newMousePos;
        pan(delta);
        lastMousePos = newMousePos;
    }
}

void Viewport::zoom(float factor) {
    view.zoom(factor);
}

void Viewport::pan(sf::Vector2f movement) {
    view.move(movement);
}

void Viewport::update() {
    window.setView(view);
}

sf::Vector2f Viewport::toWorldCoordinates(sf::Vector2i screenCoordinates) {
    return window.mapPixelToCoords(screenCoordinates);
}

sf::Vector2i Viewport::toScreenCoordinates(sf::Vector2f worldCoordinates) {
    return window.mapCoordsToPixel(worldCoordinates);
}
