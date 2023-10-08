#ifndef LIGHTS_H_
#define LIGHTS_H_

#include "Transform.h"

using glm::vec3;

/**
 * Abstract Base Class for all lights in the Scene
 *
 */
class LightSource {
  public:
        LightSource() {};
        virtual vec3 computeLight(vec3 hitPoint, vec3 directionToEye,
                          vec3 diffuse, vec3 specular, float shininess,
                          vec3 objectNormal) = 0;
        virtual void printInfo() = 0;
};

/**
 * Point light source
 *
 */
class PointLight : public LightSource {
  public:
        PointLight(float* input) :
            xyz(vec3(input[0], input[1], input[2])),
            rgb(vec3(input[3], input[4], input[5])) {}

        vec3 computeLight(vec3 hitPoint, vec3 directionToEye,
                          vec3 diffuse, vec3 specular, float shininess,
                          vec3 objectNormal);
        void printInfo();
  private:
        vec3 xyz, rgb;
};

/**
 * Directional light source
 *
 */
class DirectionalLight : public LightSource {
  public:
        DirectionalLight(float* input) :
            xyz(vec3(input[0], input[1], input[2])),
            rgb(vec3(input[3], input[4], input[5])) {}
        vec3 computeLight(vec3 hitPoint, vec3 directionToEye,
                          vec3 diffuse, vec3 specular, float shininess,
                          vec3 objectNormal);
        void printInfo();
  private:
        vec3 xyz, rgb;
};

#endif // LIGHTS_H_
