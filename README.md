# nanoraytracer

A simple recursive raytracer written in C++. Supports shadows, reflections, diffuse/specular lighting and sphere/triangle primitives.

## Usage

``` sh
make nanoraytracer
./nanoraytracer <path/to/scenefile>
```

See [demo/](demo/) for an example and info on specification of the input scenefile.

## Roadmap

- **LVL 0**
  - Add **circle** primitive
  - Create demo gif to showcase project
- **LVL 1**
  - Port from Makefile to CMake to enable cross-platform build
  - Anti-Aliasing
  - Add Logging, Progress Bar, Profiling
  - Support Refraction/Transparency
  - Parallelize with OpenMP / OpenCL
- **LVL 2**
  - Enable loading of arbitrary mesh file
  - Migrate to better representation of scene (yaml? XML?)
  - Path Tracing

## Acknowledgements

The skeleton of the code in this repo is adapted from the homework assignments of CSE167 at UCSD.
