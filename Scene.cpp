#include "Scene.h"
#include <iostream>
#include <stdexcept>

void Scene::addCamera(vec3 eye, vec3 center, vec3 up, float fovy) {
  eye = eye;
  center = center;
  up = up;
  fovy = glm::radians(fovy);
  fovx = (2 * atan( tan(fovy / 2) ) * ((float) width/ height));
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
  allVertices.push_back(vertex);
}

void Scene::addTriangleToScene(vec3& triangle) {
  allTriangles.push_back(triangle);
}

void Scene::addSphereToScene(vector<float>& sphere) {
  allSpheres.push_back(sphere);
}

Scene::Scene() {

}

Scene::~Scene() {

}
