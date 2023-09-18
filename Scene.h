#ifndef SCENE_H_
#define SCENE_H_

// Scene class to hold components of the scene to be rendered

#include <vector>
#include <string>
#include "Transform.h"

using std::vector, std::string, glm::vec3;

class Scene {
    public:
        Scene();
        virtual ~Scene();
        void addLights(string lightType, vector<float>& values);
        void addCamera(vec3 eye, vec3 center, vec3 up, float fovy);
        void setImageResolution(int w, int h);
        void addVertexToScene(vec3& vertex);
        void addTriangleToScene(vec3& triangle);
        void addSphereToScene(vector<float>& sphere);

    private:
        // Lighting parameters array
        vector<vector<float>> directionalLights; // Directional Light Params
        vector<vector<float>> pointLights; // Point Light Params

        int width, height; //Image Resolution
        // Camera params
        float fovx, fovy;
        vec3 eye, center, up;

        // Objects in the scene
        vector<vec3> allVertices; // Nx3 vertices
        vector<vec3> allTriangles; // Nx3 triangles
        vector<vector<float>> allSpheres; // Nx3 spheres
};

#endif // SCENE_H_
