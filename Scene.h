#ifndef SCENE_H_
#define SCENE_H_

// Scene class to hold components of the scene to be rendered

#include <vector>
#include <string>
#include <memory>
#include "Transform.h"
#include "SceneObjects.h"
#include "Lights.h"

using std::vector, std::string, std::shared_ptr, glm::vec3;

/**
 * Class containing all attributes of the scene - Objects, Lights, Camera
 *
 */
class Scene {
    public:
        /**
        * Add light to scene
        *
        * @param light - The light source being added to the scene
        */
        void addLight(shared_ptr<LightSource> light);
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
        * @param transform - Transformation matrix to be
        * applied to the object. Identity by default
        */
        void addObjectToScene(shared_ptr<SceneObject> sceneObj);

        // Camera params
        vec3 eye, center, up;
        float fieldOfViewX, fieldOfViewY;
        int width, height; //Image Resolution

        // Objects in the scene
        vector<shared_ptr<SceneObject>> sceneObjects;
        // Lights in the scene
        vector<shared_ptr<LightSource>> lights;
};

#endif // SCENE_H_
