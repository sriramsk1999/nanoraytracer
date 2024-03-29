// Readfile definitions
#include "Scene.h"
#include "Raytracer.h"

void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, float * values) ;
void readfile (const char * filename, Scene& scene, Raytracer& raytracer) ;
