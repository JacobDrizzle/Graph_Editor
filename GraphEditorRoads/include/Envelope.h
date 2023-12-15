#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "RoundedRectangleShape.h"
#include "Segment.h"

class Envelope {
public:
    Envelope(const Segment& skeleton, double width, int roundness = 1);
    void draw(sf::RenderWindow& window) const; // For drawing using SFML
    const Segment& getSkeleton() const;
    double getWidth() const;
    void updateSkeleton(const Segment& newSkeleton);
    void updateRoundedRect();
private:
    Segment skeleton;
    sf::RoundedRectangleShape roundedRect;
    sf::Texture texture; // Texture member
    double width;
};

#endif // ENVELOPE_H
