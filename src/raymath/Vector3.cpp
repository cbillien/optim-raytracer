#include "Vector3.hpp"

std::ostream& operator<<(std::ostream& stream, const Vector3& vec) {
  return stream << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
}
