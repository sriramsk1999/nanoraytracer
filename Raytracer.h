#ifndef RAYTRACER_H_
#define RAYTRACER_H_

// Raytracer

#include <vector>
#include <string>
#include <FreeImage.h>

#include "Transform.h"
#include "Scene.h"

using std::vector, std::string, glm::vec3;

class Raytracer {
    public:
        Raytracer(int width, int height);
        virtual ~Raytracer();
        void rayTrace(Scene& scene);
        vec3 rayCast(float iCenter, float jCenter, Scene& scene);
        bool checkIntersection(Scene& scene, vec3 rayDirection);
        void setRedColor(int i, int j);
        void saveImage(string fname);

        FIBITMAP* image;
        int width, height;
};


#endif // RAYTRACER_H_
