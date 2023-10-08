#ifndef SCENEOBJECTS_H_
#define SCENEOBJECTS_H_

#include "Transform.h"

using std::pair, std::make_pair, glm::vec3;

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
        * @param materialProps - Store material properties in base class
        * @param transform - Transform to be applied to object
        *
        */
        SceneObject(materialProperties materialProps, mat4 transform) :
                materialProps(materialProps), transform(transform) {}
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
        virtual pair<float, vec3> hitTest(vec3& eye, vec3& rayDirection) = 0;

        /**
        * Fetch surface normal of object
        *
        * @param hitPoint - Point of intersection on object
        * @return Normal of object
        */
        virtual vec3 getNorm(vec3 hitPoint) = 0;

        /**
        * Return a reference to the material properties of the object.
        *
        * @return const reference to properties of object
        */
        const materialProperties& getMaterialProperties() {
                return materialProps;
        }

        /**
        * Instead of applying the transform to the object
        * and checking for intersection, apply the inverse
        * transform to the ray. This function returns the
        * new ray params (eye and ray direction) after applying
        * inverse transform.
        *
        * @param eye - eye vector from which ray is cast
        * @param rayDirection - direction in which ray is cast
        * @return eye, rayDirection after appplying inverse object transforms
        */
        std::pair<vec3, vec3> getTransformedRay(vec3& eye, vec3& rayDirection) {
                mat4 invTransform = inverse(transform);
                vec3 transEye = vec3(invTransform * vec4(eye, 1.0));
                vec3 transDirection = normalize(vec3(invTransform * vec4(rayDirection, 0.0)));
                return std::make_pair(transEye, transDirection);
        }
  protected:
        materialProperties materialProps;
        mat4 transform;
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
        * @param transform - 4x4 transform to be applied to object
        */
        Triangle(vec3 a, vec3 b, vec3 c,
                 materialProperties materialProps,
                 mat4 transform = mat4(1.0)) :
                a(a), b(b), c(c),
                SceneObject(materialProps, transform) {}
        /**
        * Print paramters of Triangle
        *
        */
        virtual void printInfo();

        /**
        * Fetch surface normal of triangle
        *
        * @param hitPoint - Point of intersection on triangle
        * The normal of a triangle is independent of point of intersection,
        * thus, a default argument of (0,0,0) is provided.
        * @return Normal of triangle
        */
        virtual vec3 getNorm(vec3 hitPoint = vec3(0,0,0));
        /**
        * Perform hit test on triangle.
        * Check if the ray cast from eye
        * intersects with the triangle.
        *
        * @param eye - xyz location of eye
        * @param rayDirection - direction of the ray being cast
        * @return Returns a pair containing
        * 1. Distance from the eye to the triangle, or -1 if ray does not intersect.
        * 2. The point of intersection on the object
        */
        virtual pair<float, vec3> hitTest(vec3& eye, vec3& rayDirection);
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
        * @param transform - 4x4 transform to be applied to object
        */
        Sphere(float x, float y, float z, float radius,
               materialProperties materialProps,
               mat4 transform = mat4(1.0)) :
                center(vec3(x,y,z)), radius(radius),
                SceneObject(materialProps, transform) {}
        /**
        * Print paramters of Sphere
        *
        */
        virtual void printInfo();

        /**
        * Fetch surface normal of sphere
        *
        * @param hitPoint - Point of intersection on sphere
        * @return Normal of sphere wrt hitPoint
        */
        virtual vec3 getNorm(vec3 hitPoint);
        /**
        * Perform hit test on sphere.
        * Check if the ray cast from eye
        * intersects with the sphere.
        *
        * @param eye - xyz location of eye
        * @param rayDirection - direction of the ray being cast
        * @return Returns a pair containing
        * 1. Distance from the eye to the sphere, or -1 if ray does not intersect.
        * 2. The point of intersection on the object
        */
        virtual pair<float, vec3> hitTest(vec3& eye, vec3& rayDirection);

  private:
        float radius;
        vec3 center;
};

#endif // SCENEOBJECTS_H_
