#include "Scene.h"
#include <iostream>
#include <stdexcept>
#include <memory>

void Scene::addCamera(vec3 eye, vec3 center, vec3 up, float fovy) {
  Scene::eye = eye;
  Scene::center = center;
  Scene::up = up;
  Scene::fieldOfViewY = glm::radians(fovy);
  Scene::fieldOfViewX = (2 * atan( tan(fieldOfViewY / 2) * ((float) width/ height)));
}

void Scene::setImageResolution(int w, int h) {
  Scene::width = w;
  Scene::height = h;
}

void Scene::addLights(std::string lightType, std::vector<float>& lightParams) {
  if (lightType == "directional") directionalLights.push_back(lightParams);
  else if (lightType == "point") pointLights.push_back(lightParams);
  else throw std::invalid_argument( "invalid lightType: " + lightType );
}

void Scene::addObjectToScene(std::shared_ptr<SceneObject> sceneObj) {
  sceneObjects.push_back(sceneObj);
}

void Scene::addVertexToScene(vec3& vertex) {
  vertices.push_back(vertex);
}

void Scene::addTriangleToScene(vec3& triangle) {
  triangles.push_back(triangle);
}

void Scene::addSphereToScene(vector<float>& sphere) {
  spheres.push_back(sphere);
}

void Triangle::printInfo() {
  std::cout <<
    "Object Type : Triangle\n\
    Vertex A: " << a[0] << " " << a[1] << " " << a[2] << "\n\
    Vertex B: " << b[0] << " " << b[1] << " " << b[2] << "\n\
    Vertex C: " << c[0] << " " << c[1] << " " << c[2] << "\n\
    Ambient: " << ambient[0] << " " << ambient[1] << " " << ambient[2] << "\n\
    Diffuse: " << diffuse[0] << " " << diffuse[1] << " " << diffuse[2] << "\n\
    Specular: " << specular[0] << " " << specular[1] << " " << specular[2] << "\n\
    Emissive: " << emission[0] << " " << emission[1] << " " << emission[2] << "\n\
    Shininess: " << shininess << "\n";
}

float Triangle::hitTest(vec3& eye, vec3& rayDirection) {
  return 0.0;
}

void Sphere::printInfo() {
  std::cout <<
    "Object Type : Sphere\n\
    Center: " << x << " " << y << " " << z << "\n\
    Radius: " << radius << "\n\
    Diffuse: " << diffuse[0] << " " << diffuse[1] << " " << diffuse[2] << "\n\
    Specular: " << specular[0] << " " << specular[1] << " " << specular[2] << "\n\
    Emissive: " << emission[0] << " " << emission[1] << " " << emission[2] << "\n\
    Shininess: " << shininess << "\n";
}

float Sphere::hitTest(vec3& eye, vec3& rayDirection) {
  return 0.0;
}
