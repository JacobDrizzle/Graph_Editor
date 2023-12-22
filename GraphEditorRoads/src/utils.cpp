#include "utils.h"
#include <cmath>
#include <iostream>
#include "Point.h"

float distance(const Point& p1, const Point& p2) {
    //std::cout << "Distance to next point: " << (std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y))) << std::endl;
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

Point translate(const Point& original, double angle, double radius) {
    Point translated;
    translated.x = original.x + radius * cos(angle);
    translated.y = original.y + radius * sin(angle);
    return translated;
}

Point average(const Point& p1, const Point& p2) {
    return Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}

Point add(const Point& p1, const Point& p2) {
    return Point(p1.x + p2.x, p1.y + p2.y);
}

Point subtract(const Point& p1, const Point& p2) {
    return Point(p1.x - p2.x, p1.y - p2.y);
}

Point scale(const Point& p, float scaler) {
    return Point(p.x * scaler, p.y * scaler);
}
float angle(const Point& p) {
    return std::atan2(p.y, p.x);
}

// Linear Interpolation
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Get Intersection (assuming Point has a constructor Point(float x, float y))
Point getIntersection(const Point& A, const Point& B, const Point& C, const Point& D) {
    // Line AB represented as a1x + b1y = c1
    double a1 = B.y - A.y;
    double b1 = A.x - B.x;
    double c1 = a1 * (A.x) + b1 * (A.y);

    // Line CD represented as a2x + b2y = c2
    double a2 = D.y - C.y;
    double b2 = C.x - D.x;
    double c2 = a2 * (C.x) + b2 * (C.y);

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0) {
        // The lines are parallel. This is simplified by returning a default Point
        return Point(-1, -1);
    } else {
        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        return Point(x, y);
    }
}
