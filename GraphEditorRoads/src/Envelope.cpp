#include "Envelope.h"
#include <cmath>
#include "Constants.h"
#include "utils.h"
#include <iostream>

Envelope::Envelope(const Segment& skeleton, double width, int roundness): skeleton(skeleton),
                    roundedRect(sf::Vector2f(distance(skeleton.p1, skeleton.p2), width),5,5){

    sf::Vector2f midpoint((skeleton.p1.x + skeleton.p2.x) / 2.0f, (skeleton.p1.y + skeleton.p2.y) / 2.0f);
    roundedRect.setPosition(midpoint);

    if (!texture.loadFromFile("Assets/roadV.png")) {
        std::cerr << "Failed to load texture from path: Assets/roadV.png" << std::endl;
    } else {
        std::cout << "Texture loaded successfully." << std::endl;
    }

    // Set the texture to the rounded rectangle
    roundedRect.setTexture(&texture);

    // Set the origin to the center for rotation
    roundedRect.setOrigin(roundedRect.getSize().x / 2.0f, roundedRect.getSize().y / 2.0f);
    roundedRect.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));

    // Calculate the rotation angle based on the segment's orientation
    float angle = std::atan2(skeleton.p2.y - skeleton.p1.y, skeleton.p2.x - skeleton.p1.x);
    roundedRect.setRotation(angle * 180.0f / M_PI);

    // Set the corner radius and other properties
    roundedRect.setCornersRadius(width / 2.0f);

    //roundedRect.setOutlineThickness(0); // Adjust as needed
    roundedRect.setFillColor(sf::Color(0, 71, 171, 128)); // Set fill color
    roundedRect.setOutlineColor(sf::Color(0, 71, 171, 128)); // Set outline color

}

void Envelope::updateSkeleton(const Segment& newSkeleton) {
    skeleton = newSkeleton;

    float newLength = distance(skeleton.p1, skeleton.p2);
    sf::Vector2f midpoint((skeleton.p1.x + skeleton.p2.x) / 2.0f, (skeleton.p1.y + skeleton.p2.y) / 2.0f);

    // Set origin before setting position and rotation
    roundedRect.setOrigin(newLength / 2.0f, 25 / 2.0f);

    // Update the size of the rectangle
    roundedRect.setSize(sf::Vector2f(newLength, 25));

    // Update the position of the rectangle
    roundedRect.setPosition(midpoint);

    // Calculate and set the rotation of the rectangle
    float angle = std::atan2(skeleton.p2.y - skeleton.p1.y, skeleton.p2.x - skeleton.p1.x) * 180.0f / M_PI;
    roundedRect.setRotation(angle);

    // Debug output
    std::cout << "Envelope updated: Midpoint (" << midpoint.x << ", " << midpoint.y
              << "), Length: " << newLength << ", Angle: " << angle << std::endl;
}

const Segment& Envelope::getSkeleton() const {
    //std::cout << "Get Skeleton called: (" << skeleton.p1.x << "," << skeleton.p2.x << ")" << std::endl;
    return this->skeleton;
}

void Envelope::updateRoundedRect() {
    sf::Vector2f midpoint((skeleton.p1.x + skeleton.p2.x) / 2.0f, (skeleton.p1.y + skeleton.p2.y) / 2.0f);
    roundedRect.setPosition(midpoint);

    float angle = std::atan2(skeleton.p2.y - skeleton.p1.y, skeleton.p2.x - skeleton.p1.x);
    roundedRect.setRotation(angle * 180.0f / M_PI);

    // Update texture rect if needed
    roundedRect.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
}

double Envelope::getWidth() const {
    return this->width;
}

void Envelope::draw(sf::RenderWindow& window) const {
    window.draw(roundedRect);
}
