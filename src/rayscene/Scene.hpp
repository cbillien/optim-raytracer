#pragma once

#include <vector>
#include "../raymath/Ray.hpp"
#include "../raymath/Color.hpp"
#include "Light.hpp"
#include "SceneObject.hpp"

class Scene
{
private:
  std::vector<SceneObject *> objects;
  std::vector<Light *> lights;

public:
  Scene();
  ~Scene();

  Color globalAmbient;

  void add(SceneObject *object);
  void addLight(Light *light);
  std::vector<Light *> getLights();

  void prepare();
  Color raycast(const Ray &r, const Ray &cameraRay, int castCount, int maxReflections);

  bool closestIntersection(Ray &r, Intersection &closest, CullingType culling);
};
