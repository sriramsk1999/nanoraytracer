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

#include "readfile.h" // prototypes for readfile.cpp

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cerr << "Usage: nanoraytracer scenefile \n";
    exit(-1);
  }
  Scene scene;
  Raytracer raytracer;
  readfile(argv[1], scene, raytracer);

  raytracer.rayTrace(scene);

  string fname = "output.png";
  raytracer.saveImage(fname);
  return 0;
}
