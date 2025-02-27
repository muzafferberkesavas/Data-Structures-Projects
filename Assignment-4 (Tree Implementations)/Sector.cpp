#include "Sector.h"
#include "iostream"
#include "cmath"
#include <string>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        distance_from_earth = sqrt(x*x + y*y + z*z);
        int sector_number = static_cast<int>(distance_from_earth);
        sector_code = std::to_string(sector_number) + (x < 0 ? "L" : (x > 0 ? "R" : "S")) + (y < 0 ? "D" : (y > 0 ? "U" : "S")) + (z < 0 ? "B" : (z > 0 ? "F" : "S"));
}

Sector::~Sector() {
}

Sector& Sector::operator=(const Sector& other) {
    if (this != &other){
        x = other.x;
        y = other.y;
        z = other.z;
        distance_from_earth = other.distance_from_earth;
        sector_code = other.sector_code;
        left = other.left;
        right = other.right;
        parent = other.parent;
        color = other.color;
    }
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}