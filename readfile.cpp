/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  


// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

/*****************************************************************************/

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <memory>
#include "Transform.h"

using namespace std;
#include "readfile.h"

void rightmultiply(const mat4 & M, stack<mat4> &transfstack)
{
  mat4 &T = transfstack.top(); 
  T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, float* values)
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i]; 
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n"; 
      return false;
    }
  }
  return true; 
}

materialProperties initMaterialProperties(float* ambient,
                                          float* diffuse,
                                          float* specular,
                                          float* emission,
                                          float shininess) {
  materialProperties materialProps;
  materialProps.ambient = vec3(ambient[0], ambient[1], ambient[2]);
  materialProps.diffuse = vec3(diffuse[0], diffuse[1], diffuse[2]);
  materialProps.specular = vec3(specular[0], specular[1], specular[2]);
  materialProps.emission = vec3(emission[0], emission[1], emission[2]);
  materialProps.shininess = shininess;
  return materialProps;
}

void readfile(const char* filename, Scene& scene, Raytracer& raytracer)
{
  string str, cmd; 
  ifstream in;

  string outputFname = "output.png";
  int maxdepth = 5;

  vec3 eye, up, center; // Positions of eye, center, up vectors
  int w, h; // Image size
  float fovy; // FOV of image
  vector <vec3> allVertices;
  vec3 vertex;
  // Material properties
  float ambient[3] = {0, 0, 0};
  float diffuse[3] = {0, 0, 0};
  float specular[3] = {0, 0, 0};
  float emission[3] = {0, 0, 0};
  float shininess = 1;

  in.open(filename); 
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.  
    // This is done using standard STL Templates 
    stack <mat4> transfstack;
    transfstack.push(mat4(1.0));  // identity

    getline (in, str); 
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
        // Ruled out comment and blank lines 

        stringstream s(str);
        s >> cmd; 
        int i; 
        float values[10]; // Position and color for light, colors for others
        // Up to 10 params for cameras.  
        bool validinput; // Validity of input 

        // Process the lighting params
        if (cmd == "directional" or cmd == "point") {
            validinput = readvals(s, 6, values); // Position/color for lts.
            if (validinput) {
              std::shared_ptr<LightSource> l;
              if (cmd == "point") l = std::make_shared<PointLight>(values);
              else l = std::make_shared<DirectionalLight>(values);
              scene.addLight(l);
            }
        }

        // Material Commands
        // Ambient, diffuse, specular, shininess properties for each object.
        // Filling this in is pretty straightforward, so I've left it in 
        // the skeleton, also as a hint of how to do the more complex ones.
        // Note that no transforms/stacks are applied to the colors. 

        else if (cmd == "ambient") {
          validinput = readvals(s, 3, values); // colors
          if (validinput) {
            for (i = 0; i < 3; i++) {
              ambient[i] = values[i]; 
            }
          }
        } else if (cmd == "diffuse") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            for (i = 0; i < 3; i++) {
              diffuse[i] = values[i]; 
            }
          }
        } else if (cmd == "specular") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            for (i = 0; i < 3; i++) {
              specular[i] = values[i]; 
            }
          }
        } else if (cmd == "emission") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            for (i = 0; i < 3; i++) {
              emission[i] = values[i]; 
            }
          }
        } else if (cmd == "shininess") {
          validinput = readvals(s, 1, values); 
          if (validinput) {
            shininess = values[0]; 
          }
        }

        // Image Size and camera params
        else if (cmd == "size") {
          validinput = readvals(s,2,values);
          if (validinput) { 
            w = (int) values[0]; h = (int) values[1]; 
          }
          scene.setImageResolution(w, h);
        } else if (cmd == "camera") {
          validinput = readvals(s,10,values); // 10 values eye cen up fov
          if (validinput) {

            eye = vec3(values[0], values[1], values[2]);
            center = vec3(values[3], values[4], values[5]);
            up = vec3(values[6], values[7], values[8]);
            up = Transform::upvector(up, eye-center);
            fovy = values[9];

          }
        }

        // Geometry
        else if (cmd == "maxverts" or cmd == "maxvertnorms") {
          ; // Not required
        }
        else if (cmd == "vertex") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            vertex = vec3(values[0], values[1], values[2]);
          allVertices.push_back(vertex);
          }
        } else if (cmd == "tri") {
          validinput = readvals(s, 3, values);
          if (validinput) {
            auto materialProps = initMaterialProperties(ambient,
                                                        diffuse,
                                                        specular,
                                                        emission,
                                                        shininess);
            std::shared_ptr<SceneObject> tri =
              std::make_shared<Triangle>(allVertices[values[0]],
                                        allVertices[values[1]],
                                        allVertices[values[2]],
                                        materialProps,
                                        transfstack.top());
            scene.addObjectToScene(tri);
          }
        } else if (cmd == "sphere") {
          validinput = readvals(s, 4, values);
          if (validinput) {
            auto materialProps = initMaterialProperties(ambient,
                                                        diffuse,
                                                        specular,
                                                        emission,
                                                        shininess);
            std::shared_ptr<SceneObject> sphr =
              std::make_shared<Sphere>(values[0], values[1], values[2],
                                       values[3], materialProps,
                                       transfstack.top());
            scene.addObjectToScene(sphr);
          }
        }

        // Transformations
        else if (cmd == "translate") {
          validinput = readvals(s,3,values); 
          if (validinput) {
            mat4 M(1,0,0,0,
                   0,1,0,0,
                   0,0,1,0,
                   values[0],values[1],values[2],1);
            rightmultiply(M, transfstack);
          }
        }
        else if (cmd == "scale") {
          validinput = readvals(s,3,values); 
          if (validinput) {
            mat4 M(values[0],0,0,0,
                   0,values[1],0,0,
                   0,0,values[2],0,
                   0,0,0,1);
            rightmultiply(M, transfstack);
          }
        }
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values); 
          if (validinput) {
            vec3 axis(values[0], values[1], values[2]);
            axis = normalize(axis);
            mat3 R = Transform::rotate(values[3], axis);
            mat4 M(R[0][0], R[0][1], R[0][2], 0,
                   R[1][0], R[1][1], R[1][2], 0,
                   R[2][0], R[2][1], R[2][2], 0,
                   0, 0, 0, 1);
            rightmultiply(M, transfstack);
          }
        }

        // I include the basic push/pop code for matrix stacks
        else if (cmd == "pushTransform") {
          transfstack.push(transfstack.top()); 
        } else if (cmd == "popTransform") {
          if (transfstack.size() <= 1) {
            cerr << "Stack has no elements.  Cannot Pop\n"; 
          } else {
            transfstack.pop(); 
          }
        }

        else {
          cerr << "Unknown Command: " << cmd << " Skipping \n"; 
        }
      }
      getline (in, str); 
    }

    scene.addCamera(eye, center, up, fovy);
    raytracer.init(scene.width, scene.height, outputFname, maxdepth);
  } else {
    cerr << "Unable to Open Input Data File " << filename << "\n"; 
    throw 2; 
  }
}
