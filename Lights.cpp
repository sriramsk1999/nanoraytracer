#include <iostream>
#include "Lights.h"

using std::max;

void PointLight::printInfo() {
  std::cout <<
    "Light Type : Point \n\
    Location : " << xyz[0] << " " << xyz[1] << " " << xyz[2] << "\n\
    Colour: " << rgb[0] << " " << rgb[1] << " " << rgb[2] << "\n";
}

vec3 PointLight::computeLight(vec3 hitPoint, vec3 directionToEye,
                    vec3 diffuse, vec3 specular, float shininess,
                    vec3 objectNormal) {
  vec3 attenuation(1.,0.,0.);
  vec3 directionToLight = normalize(xyz - hitPoint);
  float distanceToLight = length(xyz - hitPoint);
  vec3 lightRGB = rgb/(attenuation[0] +
                        attenuation[1]*distanceToLight +
                        attenuation[2]*distanceToLight*distanceToLight
                        );
  vec3 halfVector = normalize (directionToLight + directionToEye);
  vec3 diffuseLight = diffuse * max( dot(objectNormal, directionToLight), 0.0f);
  vec3 specularLight = specular * (float) pow(max( dot(objectNormal, halfVector), 0.0f), shininess);
  lightRGB = lightRGB*(diffuseLight + specularLight);
  return lightRGB;
}

void DirectionalLight::printInfo() {
  std::cout <<
    "Light Type : Directional \n\
    Location : " << xyz[0] << " " << xyz[1] << " " << xyz[2] << "\n\
    Colour: " << rgb[0] << " " << rgb[1] << " " << rgb[2] << "\n";
}


vec3 DirectionalLight::computeLight(vec3 hitPoint, vec3 directionToEye,
                    vec3 diffuse, vec3 specular, float shininess,
                    vec3 objectNormal) {
  vec3 attenuation(1.,0.,0.);
  vec3 directionToLight = normalize(xyz);
  float distanceToLight = -1.;
  vec3 lightRGB = rgb/(attenuation[0] +
                        attenuation[1]*distanceToLight +
                        attenuation[2]*distanceToLight*distanceToLight
                        );
  vec3 halfVector = normalize (directionToLight + directionToEye);
  vec3 diffuseLight = diffuse * max( dot(objectNormal, directionToLight), 0.0f);
  vec3 specularLight = specular * (float) pow(max( dot(objectNormal, halfVector), 0.0f), shininess);
  lightRGB = lightRGB*(diffuseLight + specularLight);
  return lightRGB;
}
