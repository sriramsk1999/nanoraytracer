#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>

#include <FreeImage.h>
#include "Transform.h"
#include "Scene.h"
#include "Raytracer.h"

using namespace std;

#define MAINPROGRAM
#include "variables.h"
#include "readfile.h" // prototypes for readfile.cpp

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: nanoraytracer scenefile \n";
    exit(-1);
  }
  Scene scene;
  readfile(argv[1], scene);
  Raytracer raytracer(scene.width, scene.height);
  raytracer.rayTrace(scene);

  string fname = "output.png";
  raytracer.saveImage(fname);
  return 0;
}
