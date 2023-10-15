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
        LightSource(vec3 attenuationCoeff) :
          attenuation(attenuationCoeff) {}
        virtual vec3 computeLight(vec3 hitPoint, vec3 directionToEye,
                          vec3 diffuse, vec3 specular, float shininess,
                          vec3 objectNormal) = 0;
        virtual void printInfo() = 0;
        virtual vec3 getLightPosition() = 0;
  protected:
        vec3 attenuation;
};

/**
 * Point light source
 *
 */
class PointLight : public LightSource {
  public:
        /**
        * Initialize PointLight
        * @param input - float array containing light info
        * @param attenuation - Coefficients for light attenuation
        *
        */
        PointLight(float* input, vec3 attenuation) :
            xyz(vec3(input[0], input[1], input[2])),
            rgb(vec3(input[3], input[4], input[5])),
            LightSource(attenuation) {}

        /**
        * Compute the light of this source at a given point
        *
        * @param hitPoint - Point at which light is to be computed
        * @param directionToEye - Direction to the eye, used for specular light
        * @param diffuse - Diffuse properties of parent object of point
        * @param specular - Specular properties of parent object of point
        * @param shininess - Shininess of parent object of point
        * @param objectNormal - Surface Normal of object
        */
        vec3 computeLight(vec3 hitPoint, vec3 directionToEye,
                          vec3 diffuse, vec3 specular, float shininess,
                          vec3 objectNormal);
        /**
        * Print info about object
        *
        */
        void printInfo();
        /**
        * Print info about object
        * @return Position of light
        */
        vec3 getLightPosition() {return xyz;}
  private:
        // Light location, colour/intensity
        vec3 xyz, rgb;
};

/**
 * Directional light source
 *
 */
class DirectionalLight : public LightSource {
  public:
        /**
        * Initialize DirectionalLight
        * @param input - float array containing light info
        * @param attenuation - Coefficients for light attenuation
        *
        */
        DirectionalLight(float* input, vec3 attenuation) :
            xyz(vec3(input[0], input[1], input[2])),
            rgb(vec3(input[3], input[4], input[5])),
            LightSource(attenuation) {}
        /**
        * Compute the light of this source at a given point
        *
        * @param hitPoint - Point at which light is to be computed
        * @param directionToEye - Direction to the eye, used for specular light
        * @param diffuse - Diffuse properties of parent object of point
        * @param specular - Specular properties of parent object of point
        * @param shininess - Shininess of parent object of point
        * @param objectNormal - Surface Normal of object
        */
        vec3 computeLight(vec3 hitPoint, vec3 directionToEye,
                          vec3 diffuse, vec3 specular, float shininess,
                          vec3 objectNormal);
        /**
        * Print info about object
        *
        */
        void printInfo();
        /**
        * Print info about object
        * @return Position of light
        */
        vec3 getLightPosition() {return xyz;}
  private:
        // Light location, colour/intensity
        vec3 xyz, rgb;
};

#endif // LIGHTS_H_
