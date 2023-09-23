#ifndef SCENE_H_
#define SCENE_H_

// Scene class to hold components of the scene to be rendered

#include <vector>
#include <string>
#include "Transform.h"

using std::vector, std::string, glm::vec3;

/**
 * Abstract Base Class for all objects in the Scene
 *
 */
class SceneObject {
  public:
        /**
        * Test whether the ray defined by `rayDirection`
        * intersects with the object.
        *
        * @param eye - Location from which ray is being cast.
        * @param rayDirection - The direction of the ray being cast.
        * @return Distance of the object from the `eye`.
        */
        virtual float hitTest(vec3& eye, vec3& rayDirection) = 0;
};

/**
 * Class containing all attributes of the scene - Objects, Lights, Camera
 *
 */
class Scene {
    public:
        /**
        * Add light to scene configuration
        *
        * @param lightType - Type of light being added
        * @param values - Values holding the properties of the light
        */
        void addLights(string lightType, vector<float>& values);
        /**
        * Add camera to scene configuration
        *
        * @param eye - Location of the camera
        * @param center - Point which the camera is facing
        * @param up - Up vector of camera
        * @param fovy - Field of view in the y direction
        */
        void addCamera(vec3 eye, vec3 center, vec3 up, float fovy);
        /**
        * Set resolution of the image to be rendered
        *
        * @param w - Width
        * @param h - Height
        */
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

        // Lighting parameters array
        vector<vector<float>> directionalLights; // Directional Light Params
        vector<vector<float>> pointLights; // Point Light Params
};

/**
 * Triangle object
 *
 */
class Triangle : public SceneObject {
  public:
        /**
        * Initialize a triangle
        *
        * @param a - Triangle vertex (x,y,z)
        * @param b - Triangle vertex (x,y,z)
        * @param c - Triangle vertex (x,y,z)
        * @param ambient - Ambient color of object (r,g,b)
        * @param diffuse - Diffuse color of object (r,g,b)
        * @param specular - Specular color of object (r,g,b)
        * @param emission - Emissive color of object (r,g,b)
        * @param shininess - Shininess of object
        */
        Triangle(vec3 a, vec3 b, vec3 c,
                 float* ambient, float* diffuse,
                 float* specular, float* emission,
                 float shininess) :
                a(a), b(b), c(c),
                ambient((ambient[0], ambient[1], ambient[2])),
                diffuse((diffuse[0], diffuse[1], diffuse[2])),
                specular((specular[0], specular[1], specular[2])),
                emission((emission[0], emission[1], emission[2])),
                shininess(shininess) {}
        virtual float hitTest(vec3& eye, vec3& rayDirection);
  private:
        vec3 a,b,c;
        vec3 ambient, diffuse, specular, emission;
        float shininess;
};

/**
 * Sphere object
 *
 */
class Sphere : public SceneObject {
  public:
        /**
        * Initialize a sphere
        *
        * @param x - Center of sphere (x coord)
        * @param y - Center of sphere (y coord)
        * @param z - Center of sphere (z coord)
        * @param radius - Radius of sphere
        * @param ambient - Ambient color of object (r,g,b)
        * @param diffuse - Diffuse color of object (r,g,b)
        * @param specular - Specular color of object (r,g,b)
        * @param emission - Emissive color of object (r,g,b)
        * @param shininess - Shininess of object
        */
        Sphere(float x, float y, float z, float radius,
                 float* ambient, float* diffuse,
                 float* specular, float* emission,
                 float shininess) :
                x(x), y(y), z(z), radius(radius),
                ambient((ambient[0], ambient[1], ambient[2])),
                diffuse((diffuse[0], diffuse[1], diffuse[2])),
                specular((specular[0], specular[1], specular[2])),
                emission((emission[0], emission[1], emission[2])),
                shininess(shininess) {}
        virtual float hitTest(vec3& eye, vec3& rayDirection);
  private:
        float x, y, z, radius;
        vec3 ambient, diffuse, specular, emission;
        float shininess;
};

#endif // SCENE_H_
