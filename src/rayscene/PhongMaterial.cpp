#include <iostream>
#include <cmath>
#include "PhongMaterial.hpp"
#include "Intersection.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Intersection.hpp"

PhongMaterial::PhongMaterial()
{
}

PhongMaterial::~PhongMaterial()
{
}

Color PhongMaterial::getAmbient(Intersection *intersection)
{
  return Ambient;
}

Color PhongMaterial::render(Ray &r, Ray &camera, Intersection *intersection, Scene *scene)
{

  Color color = getAmbient(intersection) * scene->globalAmbient;

  std::vector<Light *> lights = scene->getLights();
  for (int i = 0; i < lights.size(); ++i)
  {
    Light *light = lights[i];

    // Optimisation: calculer le vecteur non normalisé d'abord, puis normaliser une seule fois
    Vector3 lightDirUnnormalized = light->GetPosition() - intersection->Position;
    double lightDirLengthSquared = lightDirUnnormalized.lengthSquared();
    
    // Éviter la division par zéro et normaliser seulement si nécessaire
    if (lightDirLengthSquared < 0.000001) continue;
    
    double lightDirLength = sqrt(lightDirLengthSquared);
    Vector3 lightDir = lightDirUnnormalized / lightDirLength; // division plus rapide que normalize()

    Vector3 origin = intersection->Position + lightDir;
    Ray lightRay(origin, lightDir);
    Intersection shadowInter;
    if (!scene->closestIntersection(lightRay, shadowInter, CULLING_BACK))
    {

      float dotProdLN = lightDir.dot(intersection->Normal);
      if (dotProdLN > 0)
      {
        color = color + (light->Diffuse * Diffuse * dotProdLN);
      }

      Vector3 R = (lightDir * -1).reflect(intersection->Normal);
      float dotProdRV = R.dot(intersection->View);
      if (dotProdRV > 0)
      {
        color = color + (light->Specular * Specular * pow(dotProdRV, Shininess));
      }
    }
  }

  return color;
}