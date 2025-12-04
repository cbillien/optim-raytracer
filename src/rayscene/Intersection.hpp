#pragma once

#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"

class Material;

class Intersection
{
public:
  Vector3 Position;
  Vector3 Normal;
  float Distance = 0;
  Ray SourceRay;
  Vector3 View;
  Material* Mat = nullptr;

  Intersection() = default;
  ~Intersection() = default;

  Intersection& operator=(const Intersection& inter) = default;
};
