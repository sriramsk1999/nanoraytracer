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

        // Camera params
        vec3 eye, center, up;
        float fieldOfViewX, fieldOfViewY;
        int width, height; //Image Resolution

        // Objects in the scene
        vector<vec3> vertices; // Nx3 vertices
        vector<vec3> triangles; // Nx3 triangles
        vector<vector<float>> spheres; // Nx3 spheres

    private:
        // Lighting parameters array
        vector<vector<float>> directionalLights; // Directional Light Params
        vector<vector<float>> pointLights; // Point Light Params

};

#endif // SCENE_H_
