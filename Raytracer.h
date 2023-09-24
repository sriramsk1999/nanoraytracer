#ifndef RAYTRACER_H_
#define RAYTRACER_H_

// Raytracer

#include <vector>
#include <string>
#include <FreeImage.h>

#include "Transform.h"
#include "Scene.h"

using std::vector, std::string, std::shared_ptr, glm::vec3;

class Raytracer {
    public:
        Raytracer(int width, int height);
        virtual ~Raytracer();
        void rayTrace(Scene& scene);
        vec3 rayCast(float iCenter, float jCenter, Scene& scene);
        int hitTest(Scene& scene, vec3 rayDirection);
        void setColor(int i, int j, shared_ptr<SceneObject> object);
        void saveImage(string fname);

        FIBITMAP* image;
        int width, height;
};


#endif // RAYTRACER_H_
