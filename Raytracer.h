#ifndef RAYTRACER_H_
#define RAYTRACER_H_

// Raytracer

#include <vector>
#include <string>
#include <FreeImage.h>

#include "Transform.h"
#include "Scene.h"

using std::vector, std::string, std::shared_ptr, glm::vec3;

/**
 * Class to instantiate a RayTracer to render a given scene
 *
 */
class Raytracer {
    public:
        /**
        * Initialize the RayTracer
        *
        * @param width - Width of output image
        * @param height - Height of output image
        */
        Raytracer(int width, int height);
        ~Raytracer();
        /**
        * Raytrace a given scene
        *
        * @param scene - Object describing the composition of the scene
        */
        void rayTrace(Scene& scene);
        /**
        * Cast a ray through a pixel into the scene
        *
        * @param iCenter - Coord of pixel (center of the pixel)
        * @param jCenter - Coord of pixel (center of the pixel)
        * @param scene - Object describing the composition of the scene
        * @return Returns the direction of the ray being cast
        */
        vec3 rayCast(float iCenter, float jCenter, Scene& scene);
        /**
        * Check if a ray intersects with the objects in the scene
        *
        * @param scene - Object describing the composition of the scene
        * @param rayDirection - Direction of the ray being cast
        * @return id of the object being hit, -1 if no object
        */
        int hitTest(Scene& scene, vec3 rayDirection);
        /**
        * Set the colour of a pixel based on the object struck by it
        *
        * @param i - Pixel coord
        * @param j - Pixel coord
        * @param objectIdx - id of the object hit by ray
        * @param scene - Object describing the composition of the scene
        */
        void setColor(int i, int j, int objectIdx,
                      Scene& scene);
        /**
        * Save the output image after raytracing
        *
        * @param fname - Save filename
        */
        void saveImage(string fname);

    private:
        FIBITMAP* image;
        int width, height;
};


#endif // RAYTRACER_H_
