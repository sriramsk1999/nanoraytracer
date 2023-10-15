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
        * @return Returns a pair containing
        * 1. The id of the object intersected, or -1 if no object
        * 2. The point of intersection on the object, or (0,0,0) if no object
        */
        pair<int, vec3> hitTest(Scene& scene, vec3 rayDirection);
        /**
        * Overload of hitTest. Supports ray being cast from arbitrary
        * source and destination. E.g. A shadow ray being cast from an object
        * to a light source.
        *
        * @param scene - Object describing the composition of the scene
        * @param source - Source location of ray being cast
        * @param destination - Destination of ray being cast
        * @return boolean indicating whether any object in the scene
        * was intersected by the ray
        */
        bool hitTest(Scene& scene, vec3 source, vec3 destination);
        /**
        * Set the colour of a pixel based on the object struck by it
        *
        * @param i - Pixel coord
        * @param j - Pixel coord
        * @param objectIdx - id of the object hit by ray
        * @param hitPoint - The point on the object hit by ray
        * @param scene - Scene object containing composition of scene
        */
        void setColor(int i, int j, int objectIdx, vec3 hitPoint,
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
        int maxdepth;
        string fname;
};


#endif // RAYTRACER_H_
