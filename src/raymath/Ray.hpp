#pragma once

#include <iostream>
#include "Vector3.hpp"

class Ray
{
private:
  Vector3 position;
  Vector3 direction;
// optimisation 2 avec les const
public:
  Ray();
  Ray(const Vector3 &pos, const Vector3 &dir);
  ~Ray();

  Vector3 GetPosition() const;
  void SetPosition(const Vector3 &pos);

  Vector3 GetDirection() const;
  void SetDirection(const Vector3 &pos);

  friend std::ostream &operator<<(std::ostream &_stream, const Ray &vec);
};
