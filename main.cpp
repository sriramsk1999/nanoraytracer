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

void colorizeDummyImage(FIBITMAP* img) {
  RGBQUAD color;
  for (int i=0; i < w; i++) {
    for (int j=0; j < h; j++) {
      color.rgbRed = 0;
      color.rgbGreen = (double)i/w*255.0;
      color.rgbBlue = (double)j/h*255.0;
      FreeImage_SetPixelColor(img, i, j, &color);
    }
  }
}

vec3 raycast(float i_loc, float j_loc) {
  float alpha, beta;

  alpha = tan( glm::radians (fovx / 2) ) * (j_loc - (w/2))/(w/2);
  beta = tan( glm::radians (fovy / 2) ) * ((h/2) - j_loc)/(h/2);

  vec3 w_vec = normalize(eye-center);
  vec3 u_vec = normalize(cross(up, w_vec));
  vec3 v_vec = cross(w_vec, u_vec);

  vec3 ray_direction = normalize( alpha*u_vec + beta*v_vec - w_vec);
  return ray_direction;
}

void check_intersection(vec3 eye, vec3 ray_dirn) {
  ;
}

void raytrace(FIBITMAP* img, int w, int h) {
  float i_loc, j_loc;
  vec3 ray_dirn;
  for (int i=0; i < w; i++) {
    for (int j=0; j < h; j++) {
      i_loc = i+0.5; j_loc = j+0.5; // cast ray through center of pixel

      ray_dirn = raycast(i_loc, j_loc);
      check_intersection(eye, ray_dirn);
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
