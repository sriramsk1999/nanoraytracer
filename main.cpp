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

  FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);

  FreeImage_DeInitialise();
  return 0;
}
