#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

#include "Transform.h"
#include <FreeImage.h>

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

vec3 rayCast(float i_loc, float j_loc) {
  float alpha, beta;

  alpha = tan( glm::radians (fovx / 2) ) * (i_loc - (w/2))/(w/2);
  beta = tan( glm::radians (fovy / 2) ) * ((h/2) - j_loc)/(h/2);

  vec3 w_vec = normalize(eye-center);
  vec3 u_vec = normalize(cross(up, w_vec));
  vec3 v_vec = cross(w_vec, u_vec);

  vec3 ray_direction = normalize( alpha*u_vec + beta*v_vec - w_vec);
  return ray_direction;
}

bool checkIntersection(vec3 eye, vec3 ray_dirn) {
  vec3 a, b, c, triNorm, hitPoint;
  float ray2Plane, alpha, beta, gamma, triArea;
  bool isIntersect = false;
  for (int i = 0; i < triangles.size(); i++) {
    // Fetch vertices of triangle
    a = vertices[triangles[i][0]];
    b = vertices[triangles[i][1]];
    c = vertices[triangles[i][2]];
    triNorm = normalize( cross(c-a, b-a) );

    // ray_dirn = vec3(0,0,-1);

    ray2Plane = ( dot(a, triNorm) - dot(eye, triNorm) ) / dot( ray_dirn, triNorm );
    hitPoint = eye + ray_dirn*ray2Plane;
    // std::cout << hitPoint[0] << " " << hitPoint[1] << " " << hitPoint[2] << "\n";
    // std::cout << ray_dirn[0] << " " << ray_dirn[1] << " " << ray_dirn[2] << "\n";

    // triArea = length(cross(b-a, c-a))/2;
    // alpha = length(cross(b-hitPoint, c-hitPoint)) / (2*triArea);
    // beta  = length(cross(c-hitPoint, a-hitPoint)) / (2*triArea);
    // gamma = 1 - alpha - beta;
    // if (alpha >=0 and alpha < 1 and beta >=0 and beta < 1 and gamma >=0 and gamma < 1) {
    //   std::cout << "hitPoint " << hitPoint[0] << " " << hitPoint[1] << " " << hitPoint[2] << "\n";
    //   isIntersect = true;
    // }
    if (hitPoint[0] >= -1 and hitPoint[0] <= 1 and hitPoint[1] >=-1 and hitPoint[0] <=1) {
      //std::cout << "hitPoint " << hitPoint[0] << " " << hitPoint[1] << " " << hitPoint[2] << "\n";
      isIntersect=true;
    }
  }
  // std::cout << "------\n";
  return isIntersect;
}

void raytrace(FIBITMAP* img, int w, int h) {
  float i_loc, j_loc;
  vec3 rayDirn;
  bool isIntersect;
  for (int i=0; i < w; i++) {
    for (int j=0; j < h; j++) {
      i_loc = i+0.5; j_loc = j+0.5; // cast ray through center of pixel

      rayDirn = rayCast(i_loc, j_loc);
      isIntersect = checkIntersection(eye, rayDirn);
      if (isIntersect) setRedColor(img, i, j);
    }
  }
  return ;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: nanoraytracer scenefile \n";
    exit(-1);
  }
  FreeImage_Initialise();
  readfile(argv[1]) ;

  int bitsPerPixel = 24;
  FIBITMAP* img = FreeImage_Allocate(w, h, bitsPerPixel);
  string fname = "output.png";

  raytrace(img, w, h);

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);

  FreeImage_DeInitialise();
  return 0;
}
