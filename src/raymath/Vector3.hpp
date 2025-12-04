#pragma once

#include <iostream>
#include <cmath>

#define COMPARE_ERROR_CONSTANT 0.000001

class Vector3
{
public:
  double x = 0;
  double y = 0;
  double z = 0;

  Vector3() = default;
  Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
  ~Vector3() = default;

  inline Vector3 operator+(const Vector3& vec) const {
    return Vector3(x + vec.x, y + vec.y, z + vec.z);
  }

  inline Vector3 operator-(const Vector3& vec) const {
    return Vector3(x - vec.x, y - vec.y, z - vec.z);
  }

  inline Vector3 operator*(double f) const {
    return Vector3(x * f, y * f, z * f);
  }

  inline Vector3 operator/(double f) const {
    return Vector3(x / f, y / f, z / f);
  }

  inline Vector3& operator=(const Vector3& vec) = default;

  inline double lengthSquared() const {
    return x * x + y * y + z * z;
  }

  inline double length() const {
    return std::sqrt(lengthSquared());
  }

  inline Vector3 normalize() const {
    double len = length();
    return (len == 0) ? Vector3() : *this / len;
  }

  inline double dot(const Vector3& vec) const {
    return x * vec.x + y * vec.y + z * vec.z;
  }

  inline Vector3 projectOn(const Vector3& vec) const {
    return vec * dot(vec);
  }

  inline Vector3 reflect(const Vector3& normal) const {
    return *this + projectOn(normal) * -2;
  }

  inline Vector3 cross(const Vector3& b) const {
    return Vector3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
  }

  inline Vector3 inverse() const {
    return Vector3(1.0 / x, 1.0 / y, 1.0 / z);
  }

  friend std::ostream& operator<<(std::ostream& stream, const Vector3& vec);
};
