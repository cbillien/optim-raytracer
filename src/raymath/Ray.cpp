#include <iostream>
#include "Ray.hpp"

Ray::Ray() : position(Vector3()), direction(Vector3(0, 0, 1)) {}

Ray::Ray(const Vector3 &pos, const Vector3 &dir) : position(pos), direction(dir.normalize()) {}

Ray::~Ray() {}

Vector3 Ray::GetPosition() const { return position; }  // <-- const
void Ray::SetPosition(const Vector3 &pos) { position = pos; }

Vector3 Ray::GetDirection() const { return direction; } // <-- const
void Ray::SetDirection(const Vector3 &dir) { direction = dir.normalize(); }

std::ostream &operator<<(std::ostream &_stream, const Ray &ray) {
    return _stream << "Ray(" << ray.GetPosition() << ", " << ray.GetDirection() << ")";
}
