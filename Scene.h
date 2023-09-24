#ifndef SCENE_H_
#define SCENE_H_

// Scene class to hold components of the scene to be rendered

#include <vector>
#include <string>
#include <memory>
#include "Transform.h"

using std::vector, std::string, std::shared_ptr, glm::vec3;

/**
 * Store the various material properties of an object.
 * Used for lighting the object correctly.
 *
 */
struct materialProperties {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec3 emission;
        float shininess;
};

/**
 * Abstract Base Class for all objects in the Scene
 *
 */
class SceneObject {
  public:
        /**
        * Initialize SceneObject
        * Store material properties in base class
        *
        */
        SceneObject(materialProperties materialProps) :
                materialProps(materialProps) {}
        /**
        * Print info about object
        *
        */
        virtual void printInfo() = 0;
        /**
        * Test whether the ray defined by `rayDirection`
        * intersects with the object.
        *
        * @param eye - Location from which ray is being cast.
        * @param rayDirection - The direction of the ray being cast.
        * @return Distance of the object from the `eye`.
        */
        virtual float hitTest(vec3& eye, vec3& rayDirection) = 0;
        /**
        * Return a reference to the material properties of the object.
        *
        * @return const reference to properties of object
        */
        const materialProperties& getMaterialProperties() {
                return materialProps;
        }
  protected:
        materialProperties materialProps;
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

        /**
        * Add an object to the scene
        *
        * @param sceneObj - Pointer to an object
        * implementing the SceneObj interface
        */
        void addObjectToScene(shared_ptr<SceneObject> sceneObj);

        // Camera params
        vec3 eye, center, up;
        float fieldOfViewX, fieldOfViewY;
        int width, height; //Image Resolution

        // Objects in the scene
        vector<shared_ptr<SceneObject>> sceneObjects;

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
        * @param materialProps - Material properties of object, used for lighting.
        */
        Triangle(vec3 a, vec3 b, vec3 c,
                 materialProperties materialProps) :
                a(a), b(b), c(c),
                SceneObject(materialProps) {}
        /**
        * Print paramters of Triangle
        *
        */
        virtual void printInfo();
        /**
        * Perform hit test on triangle.
        * Check if the ray cast from eye
        * intersects with the triangle.
        *
        * @param eye - Emissive color of object (r,g,b)
        * @param rayDirection - Shininess of object
        * @return Returns the distance from the eye to the triangle,
        * or -1 if ray does not intersect.
        */
        virtual float hitTest(vec3& eye, vec3& rayDirection);
  private:
        vec3 a,b,c;
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
        * @param materialProps - Material properties of object, used for lighting.
        */
        Sphere(float x, float y, float z, float radius,
               materialProperties materialProps) :
                center(vec3(x,y,z)), radius(radius),
                SceneObject(materialProps) {}
        /**
        * Print paramters of Sphere
        *
        */
        virtual void printInfo();
        virtual float hitTest(vec3& eye, vec3& rayDirection);

  private:
        float radius;
        vec3 center;
};

#endif // SCENE_H_
