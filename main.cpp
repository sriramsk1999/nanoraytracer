#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

#include <FreeImage.h>
#include "Transform.h"
#include "Scene.h"

using namespace std;

#define MAINPROGRAM
#include "variables.h"
#include "readfile.h" // prototypes for readfile.cpp

void setRedColor(FIBITMAP* img, int i, int j) {
  RGBQUAD color;
  color.rgbRed = 255;
  color.rgbGreen = 0;
  color.rgbBlue = 0;
  FreeImage_SetPixelColor(img, i, j, &color);
}

vec3 rayCast(float i_loc, float j_loc, Scene& scene) {
  float alpha, beta;

  alpha = tan( (scene.fieldOfViewX / 2) ) * (i_loc - (scene.width/2))/(scene.width/2);
  beta = tan( (scene.fieldOfViewY / 2) ) * ((scene.height/2) - j_loc)/(scene.height/2);

  vec3 w_vec = normalize(scene.eye-scene.center);
  vec3 u_vec = normalize(cross(scene.up, w_vec));
  vec3 v_vec = cross(w_vec, u_vec);

  vec3 ray_direction = normalize( alpha*u_vec + beta*v_vec - w_vec);
  return ray_direction;
}

bool checkIntersection(vec3 eye, vec3 ray_dirn, Scene& scene) {
  vec3 a, b, c, triNorm, hitPoint;
  float ray2Plane, pointA, pointB, pointC;
  bool isIntersect = false;
  for (int i = 0; i < scene.triangles.size(); i++) {
    // Fetch vertices of triangle
    a = scene.vertices[scene.triangles[i][0]];
    b = scene.vertices[scene.triangles[i][1]];
    c = scene.vertices[scene.triangles[i][2]];
    triNorm = normalize( cross(c-a, b-a) );

    ray2Plane = ( dot(a, triNorm) - dot(eye, triNorm) ) / dot( ray_dirn, triNorm );
    hitPoint = scene.eye + ray_dirn*ray2Plane;

    pointA = cross(b-a, hitPoint-a)[2];
    pointB = cross(c-b, hitPoint-b)[2];
    pointC = cross(a-c, hitPoint-c)[2];

    if ((pointA>0 and pointB>0 and pointC>0) or (pointA<=0 and pointB<=0 and pointC<=0)) {
      isIntersect=true;
    }
  }
  return isIntersect;
}

void raytrace(FIBITMAP* img, int w, int h, Scene& scene) {
  float i_loc, j_loc;
  vec3 rayDirn;
  bool isIntersect;
  for (int i=0; i < w; i++) {
    for (int j=0; j < h; j++) {
      i_loc = i+0.5; j_loc = j+0.5; // cast ray through center of pixel

      rayDirn = rayCast(i_loc, j_loc, scene);
      isIntersect = checkIntersection(eye, rayDirn, scene);
      if (isIntersect) setRedColor(img, i, h-j);
    }
  }
  return ;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: nanoraytracer scenefile \n";
    exit(-1);
  }
  Scene scene;
  FreeImage_Initialise();
  readfile(argv[1], scene);

  int bitsPerPixel = 24;
  FIBITMAP* img = FreeImage_Allocate(w, h, bitsPerPixel);
  string fname = "output.png";

  raytrace(img, w, h, scene);

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);

  FreeImage_DeInitialise();
  return 0;
}
