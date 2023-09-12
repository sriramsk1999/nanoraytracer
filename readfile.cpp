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
#include "Transform.h"

using namespace std;
#include "variables.h"
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

void readfile(const char* filename) 
{
  string str, cmd; 
  ifstream in;
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
            std::vector<float> lightParams;
            if (validinput) {
              for (i = 0; i < 6; i++) {
                lightParams.push_back(values[i]);
              }

              if (cmd == "directional") directionalLights.push_back(lightParams);
              else pointLights.push_back(lightParams);
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
        } else if (cmd == "camera") {
          validinput = readvals(s,10,values); // 10 values eye cen up fov
          if (validinput) {

            eyeinit = vec3(values[0], values[1], values[2]);
            center = vec3(values[3], values[4], values[5]);
            upinit = vec3(values[6], values[7], values[8]);
            upinit = Transform::upvector(upinit, eyeinit-center);
            fovy = values[9];

          }
        }

        // I've left the code for loading objects in the skeleton, so 
        // you can get a sense of how this works.  
        // Also look at demo.txt to get a sense of why things are done this way.
        else if (cmd == "sphere" || cmd == "cube" || cmd == "teapot") {
          if (numobjects == maxobjects) { // No more objects 
            cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
          } else {
            validinput = readvals(s, 1, values); 
            if (validinput) {
              object * obj = &(objects[numobjects]); 
              obj->size = values[0]; 

              // Set the object's light properties
              for (i = 0; i < 4; i++) {
                (obj->ambient)[i] = ambient[i]; 
                (obj->diffuse)[i] = diffuse[i]; 
                (obj->specular)[i] = specular[i]; 
                (obj->emission)[i] = emission[i];
              }
              obj->shininess = shininess; 

              // Set the object's transform
              obj->transform = transfstack.top(); 

              // Set the object's type
              if (cmd == "sphere") {
                obj->type = sphere; 
              } else if (cmd == "cube") {
                obj->type = cube; 
              } else if (cmd == "teapot") {
                obj->type = teapot; 
              }
            }
            ++numobjects; 
          }
        }

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

    // Set up initial position for eye, up and amount
    // As well as booleans 

    eye = eyeinit; 
    up = upinit; 
    amount = amountinit;
    sx = sy = 1.0;  // keyboard controlled scales in x and y 
    tx = ty = 0.0;  // keyboard controllled translation in x and y  
  } else {
    cerr << "Unable to Open Input Data File " << filename << "\n"; 
    throw 2; 
  }
}
