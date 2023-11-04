# Demo

Generate an image of the Stanford Dragon with:

``` sh
./nanoraytracer demo/stanford-dragon.test
```

## Description of scenefile

*Adapted from the description of the homework for CSE167 at UCSD.*

- `size width height`: The size command must be the first command of the file, which controls the image size.
- `maxdepth depth`: The maximum depth (number of bounces) for a ray (default 5).
- `output filename`: The output file to which the image should be written. (default output.png).
- `camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov`: specifies the camera using its look-from coords, look-at coords and the up-vector. fov stands for the field of view in the y direction. The field of view in the x direction will be determined by the image size.
- `sphere x y z radius`: Defines a sphere with a given position and radius.
- `vertex x y z`: Defines a vertex at the given location. The vertex is put into a pile, starting to be numbered at 0.
- `tri v1 v2 v3`: Create a triangle out of the vertices involved (which have previously been specified with the vertex command). The vertices are assumed to be specified in counter-clockwise order. 
- `translate x y z`: A translation 3-vector.
- `rotate x y z angle`: Rotate by angle (in degrees) about the given axis as in OpenGL.
- `scale x y z`: Scale by the corresponding amount in each axis (a non-uniform scaling).
- `pushTransform`: Push the current modeling transform on the stack as in OpenGL. You might want to do pushTransform immediately after setting the camera to preserve the identity transformation.
- `popTransform`: Pop the current transform from the stack as in OpenGL. 
- `directional x y z r g b`: The direction to the light source, and the color, as in OpenGL.
- `point x y z r g b`: The location of a point source and the color, as in OpenGL.
- `attenuation const linear quadratic`: Sets the constant, linear and quadratic attenuations (default 1,0,0) as in OpenGL. 
- `ambient r g b`: The global ambient color to be added for each object.
- `diffuse r g b`: specifies the diffuse color of the surface.
- `specular r g b`: specifies the specular color of the surface.
- `shininess s`: specifies the shininess of the surface.
- `emission r g b`: gives the emissive color of the surface.

