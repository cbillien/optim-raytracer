#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "../raymath/Vector3.hpp"

Sphere::Sphere(double r) : SceneObject(), radius(r)
{
}

Sphere::~Sphere()
{
}

void Sphere::applyTransform()
{
  Vector3 c;
  this->center = this->transform.apply(c);
}

bool Sphere::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  // Vector from ray origin to center of sphere
  Vector3 OC = center - r.GetPosition();

  // Project OC onto the ray
  Vector3 OP = OC.projectOn(r.GetDirection());

  // If the OP vector is pointing in the opposite direction of the ray
  // ... then it is behind the ray origin, ignore the object
  if (OP.dot(r.GetDirection()) <= 0)
  {
    return false;
  }

  // P is the corner of the right-angle triangle formed by O-C-P
  Vector3 P = r.GetPosition() + OP;

  // Is the length of CP greater than the radius of the circle ? If yes, no intersection!
  // Optimisation: utiliser lengthSquared() au lieu de length() pour éviter sqrt
  Vector3 CP = P - center;
  double distanceSquared = CP.lengthSquared();
  double radiusSquared = radius * radius;
  if (distanceSquared > radiusSquared)
  {
    return false;
  }

  // Calculate the exact point of collision: P1
  // Optimisation: utiliser distanceSquared au lieu de distance pour éviter sqrt
  double a = sqrt(radiusSquared - distanceSquared);
  // Optimisation: calculer OP.lengthSquared() et prendre sqrt seulement une fois
  double OP_length = sqrt(OP.lengthSquared());
  double t = OP_length - a;
  Vector3 P1 = r.GetPosition() + (r.GetDirection() * t);

  // Pre-calculate some useful values for rendering
  intersection.Position = P1;
  intersection.Mat = this->material;
  // Optimisation: calculer le vecteur une fois, puis normaliser
  Vector3 toCenter = P1 - center;
  intersection.Normal = toCenter.normalize();

  return true;
}
