#include "Scene.h"
#include <iostream>
#include <stdexcept>

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

void Scene::addVertexToScene(vec3& vertex) {
  vertices.push_back(vertex);
}

void Scene::addTriangleToScene(vec3& triangle) {
  triangles.push_back(triangle);
}

void Scene::addSphereToScene(vector<float>& sphere) {
  spheres.push_back(sphere);
}

Scene::Scene() {

}

Scene::~Scene() {

}
