#include "utils.h"
#include <cmath>
#include <iostream>

float distance(const Point& p1, const Point& p2) {
    std::cout << "Distance to next point: " << (std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y))) << std::endl;
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
