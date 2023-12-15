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
Point* getIntersection(const Point& A, const Point& B, const Point& C, const Point& D) {
    float tTop = (D.x - C.x) * (A.y - C.y) - (D.y - C.y) * (A.x - C.x);
    float uTop = (C.y - A.y) * (A.x - B.x) - (C.x - A.x) * (A.y - B.y);
    float bottom = (D.y - C.y) * (B.x - A.x) - (D.x - C.x) * (B.y - A.y);

    if (bottom != 0) {
        float t = tTop / bottom;
        float u = uTop / bottom;
        if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
            return new Point(lerp(A.x, B.x, t), lerp(A.y, B.y, t));
        }
    }

    return nullptr;
}
