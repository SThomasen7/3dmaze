#ifndef _CAMERA_COMPONENT_H_
#define _CAMERA_COMPONENT_H_ 1

#include <glm/vec3.hpp>

enum CameraProjection{
  Perspective,
  Orthographic
};

struct CameraComponent{

  glm::vec3 up;
  glm::vec3 lookat;
  glm::vec3 right;
  CameraProjection projection_type;

  float fov;
  float buffer_x, buffer_y;
  float z_near, z_far;

  float pitch, yaw, roll;

};

#endif
