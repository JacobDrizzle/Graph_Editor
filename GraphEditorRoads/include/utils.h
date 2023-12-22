#ifndef UTILS_H
#define UTILS_H

#include "Point.h"

float distance(const Point& p1, const Point& p2);
Point translate(const Point& original, double angle, double radius);
Point average(const Point& p1, const Point& p2);

Point add(const Point& p1, const Point& p2);

Point subtract(const Point& p1, const Point& p2);

Point scale(const Point& p, float scaler);

// The translate function is already defined in your C++ code

float angle(const Point& p);

// Linear Interpolation
float lerp(float a, float b, float t);

// Get Intersection (assuming Point has a constructor Point(float x, float y))
Point getIntersection(const Point& A, const Point& B, const Point& C, const Point& D);

#endif // UTILS_H
