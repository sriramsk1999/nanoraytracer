// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
  float radians = (degrees * pi)/180.0;
  mat3 M, I(1.0);
  mat3 M1 = I*float(cos(radians));
  mat3 M2(axis[0]*axis[0], axis[1]*axis[0], axis[2]*axis[0],
          axis[0]*axis[1], axis[1]*axis[1], axis[2]*axis[1],
          axis[0]*axis[2], axis[1]*axis[2], axis[2]*axis[2]);
  M2 = M2*(1 - float(cos(radians)));
  mat3 M3(0, axis[2], -axis[1],
          -axis[2], 0, axis[0],
          axis[1], -axis[0], 0);
  M3 = M3 * float(sin(radians));
  M = M1 + M2 + M3;
  return M;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
  mat3 M = rotate(degrees, up);
  eye = M * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
  vec3 axisForUpRot = normalize(cross(eye, up));
  mat3 M = rotate(degrees, axisForUpRot);
  eye = M * eye;
  up = M * up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
  vec3 w = normalize(eye);
  vec3 u = normalize(cross(up,w));
  vec3 v = cross(w, u);

  mat4 R(u[0], v[0], w[0], 0, u[1], v[1], w[1], 0, u[2], v[2], w[2], 0, 0, 0, 0, 1);
  mat4 T(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -eye[0], -eye[1], -eye[2], 1);
  mat4 RT = R*T;
  return RT;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
  float d = 1/tan((fovy*pi/180)/2);
  float a = (-zFar + zNear)/(zFar-zNear);
  float b = (-2*zFar*zNear)/(zFar-zNear);
  mat4 ret(d/aspect,0,0,0,
           0,d,0,0,
           0,0,a,-1,
           0,0,b,0);
  return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
  mat4 ret(sx, 0, 0, 0,
           0, sy, 0, 0,
           0, 0, sz, 0,
           0, 0, 0,  1);
  return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
  mat4 ret(1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1, 0,
           tx, ty, tz,  1);
  return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
