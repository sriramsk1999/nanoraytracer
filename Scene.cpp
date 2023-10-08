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

void Scene::addLight(std::shared_ptr<LightSource> light) {
  lights.push_back(light);
}

void Scene::addObjectToScene(std::shared_ptr<SceneObject> sceneObj) {
  sceneObjects.push_back(sceneObj);
}

