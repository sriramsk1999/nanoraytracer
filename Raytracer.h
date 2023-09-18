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
        Raytracer();
        virtual ~Raytracer();
        void rayTrace(Scene& scene);
        void rayCast(float iCenter, float jCenter);
        bool checkIntersection(vec3 rayDirection);
        void setRedColor(int i, int j);

        FIBITMAP* image;
        int width, height; //Image Resolution
};


#endif // RAYTRACER_H_
