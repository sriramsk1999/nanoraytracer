#ifndef RAYTRACER_H_
#define RAYTRACER_H_

// Raytracer

#include <vector>
#include <string>
#include <FreeImage.h>

#include "Transform.h"
#include "Scene.h"

using std::vector, std::string, std::shared_ptr, std::max, glm::vec3;

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
        * @param fname - Name of output file
        * @param maxdepth - Maximum number of bounces for a ray
        */
        void init(int width=640, int height=480,
                  string fname="output.png",
                  int maxdepth=5);
        ~Raytracer();
        /**
        * Raytrace a given scene
        *
        * @param scene - Object describing the composition of the scene
        */
        void rayTrace(Scene& scene);
        /**
        * Recursively raytrace a single ray
        *
        * @param scene - Object describing the composition of the scene
        * @param eye - Vector describing eye location
        * @param rayDirection - Vector describing direction of ray
        * @param currentDepth - Number of times ray has bounced
        * @return The colour visible from this ray
        */
        vec3 recursiveRayTrace(Scene& scene, vec3 eye,
                               vec3 rayDirection, int currentDepth);
        /**
        * Compute the colour from a single raytrace (without reflections)
        *
        * @param scene - Object describing the composition of the scene
        * @param eye - Vector describing eye location
        * @param rayDirection - Vector describing direction of ray
        * @return The colour visible from this ray without considering reflections
        */
        vec3 computeColorAtPoint(Scene& scene, int objectIdx, vec3 eye, vec3 hitPoint);
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
        * @param eye - Vector describing eye location
        * @param rayDirection - Direction of the ray being cast
        * @return Returns a pair containing
        * 1. The id of the object intersected, or -1 if no object
        * 2. The point of intersection on the object, or (0,0,0) if no object
        */
        pair<int, vec3> hitTest(Scene& scene, vec3 eye, vec3 rayDirection);
        /**
        * Checks if light is visible from given eye location.
        * Used to implement shadows.
        *
        * @param scene - Object describing the composition of the scene
        * @param eye - Point from which the check is being made
        * @param lightpos - The position of a given light
        * @return boolean indicating whether the light is visible from eye
        */
        bool isLightVisible(Scene& scene, vec3 eye, vec3 lightpos);
        /**
        * Set the colour of a pixel
        *
        * @param RGB - Colour to be set
        * @param i - Pixel coord
        * @param j - Pixel coord
        */
        void setColor(vec3 RGB, int i, int j);
        /**
        * Save the output image after raytracing
        *
        */
        void saveImage();

    private:
        FIBITMAP* image;
        int width, height;
        int maxdepth;
        string fname;
};


#endif // RAYTRACER_H_
