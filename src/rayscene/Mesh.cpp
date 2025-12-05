#include <iostream>
#include "Mesh.hpp"
#include "../raymath/Vector3.hpp"
#include "../objloader/OBJ_Loader.h"

Mesh::Mesh() : SceneObject()
{
}

Mesh::~Mesh()
{
    for (int i = 0; i < triangles.size(); ++i)
    {
        delete triangles[i];
    }
}

void Mesh::loadFromObj(std::string path)
{

    objl::Loader *loader = new objl::Loader();
    bool loadout = loader->LoadFile(path);

    if (loadout)
    {
        for (int i = 0; i < loader->LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = loader->LoadedMeshes[i];

            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                Vector3 v1(
                    curMesh.Vertices[curMesh.Indices[j]].Position.X,
                    curMesh.Vertices[curMesh.Indices[j]].Position.Y,
                    curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                Vector3 v2(
                    curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
                    curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
                    curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
                Vector3 v3(
                    curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
                    curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
                    curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);

                Triangle *triangle = new Triangle(
                    v1,
                    v2,
                    v3);
                triangle->name = "T:" + std::to_string(j);
                triangle->ID = j;
                triangles.push_back(triangle);
            }
        }
    }

    this->applyTransform();
    delete loader;
}

void Mesh::applyTransform()
{
#ifdef USE_AABB
    // Initialiser la bounding box avec des valeurs extrêmes
    bool first = true;
#endif
    
    for (int i = 0; i < triangles.size(); ++i)
    {
        triangles[i]->material = this->material;
        triangles[i]->transform = transform;
        triangles[i]->applyTransform();

#ifdef USE_AABB
        // Agrandir la bounding box du mesh pour inclure chaque triangle
        if (first)
        {
            boundingBox = triangles[i]->boundingBox;
            first = false;
        }
        else
        {
            boundingBox.subsume(triangles[i]->boundingBox);
        }
#endif
    }
}

bool Mesh::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
#ifdef USE_AABB
    // Early rejection : si le rayon ne touche pas la bounding box du mesh,
    // on évite de tester les centaines de triangles !
    if (!boundingBox.intersects(r))
    {
        return false;
    }
#endif

    Intersection tInter;

    double closestDistance2 = -1; // distance au carré pour éviter sqrt
    Intersection closestInter;
    for (int i = 0; i < triangles.size(); ++i)
    {
        if (triangles[i]->intersects(r, tInter, culling))
        {
            // On calcule la distance au carré pour éviter le sqrt
            double dist2 = (tInter.Position - r.GetPosition()).lengthSquared();
            if (closestDistance2 < 0 || dist2 < closestDistance2)
            {
                closestDistance2 = dist2;
                closestInter = tInter;
            }
        }
    }

    if (closestDistance2 < 0)
    {
        return false;
    }

    intersection = closestInter;
    return true;
}