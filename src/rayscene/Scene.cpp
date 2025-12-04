#include <iostream>
#include "Scene.hpp"
#include <cmath> 
#include "Intersection.hpp"


Scene::Scene() {}
Scene::~Scene()
{
    for (SceneObject* obj : objects) delete obj;
    for (Light* light : lights) delete light;
}

void Scene::add(SceneObject *object)
{
    objects.push_back(object);
}

void Scene::addLight(Light *light)
{
    lights.push_back(light);
}

void Scene::prepare()
{
    for (SceneObject* obj : objects) obj->applyTransform();
}

std::vector<Light *> Scene::getLights()
{
    return lights;
}


bool Scene::closestIntersection(Ray &r, Intersection &closest, CullingType culling)
{
    Intersection intersection;
    double closestDistance2 = -1;  // distance au carré
    Intersection closestInter;

    for (SceneObject* obj : objects)
    {
        if (obj->intersects(r, intersection, culling))
        {
            // On calcule la distance au carré pour éviter le sqrt
            double dist2 = (intersection.Position - r.GetPosition()).lengthSquared();
            
            if (closestDistance2 < 0 || dist2 < closestDistance2)
            {
                closestDistance2 = dist2;
                closestInter = intersection;
                closestInter.Distance = std::sqrt(dist2); // on garde Distance réelle si besoin
            }
        }
    }

    closest = closestInter;
    return (closestDistance2 > -1);
}


Color Scene::raycast(const Ray &r, const Ray &camera, int castCount, int maxCastCount)
{
    Color pixel;
    Intersection intersection;

    // On cherche l'intersection la plus proche
    if (closestIntersection(const_cast<Ray&>(r), intersection, CULLING_FRONT))
    {
        // Calcul du vecteur de vue
        intersection.View = (camera.GetPosition() - intersection.Position).normalize();

        if (intersection.Mat != nullptr)
        {
            // Rendu du matériau
            pixel = pixel + intersection.Mat->render(const_cast<Ray&>(r), const_cast<Ray&>(camera), &intersection, this);

            // Réflexion si nécessaire
            if (castCount < maxCastCount && intersection.Mat->cReflection > 0)
            {
                Vector3 reflectDir = r.GetDirection().reflect(intersection.Normal);
                Vector3 origin = intersection.Position + (reflectDir * COMPARE_ERROR_CONSTANT);
                Ray reflectRay(origin, reflectDir);

                pixel = pixel + raycast(reflectRay, camera, castCount + 1, maxCastCount) * intersection.Mat->cReflection;
            }
        }
    }

    return pixel;
}
