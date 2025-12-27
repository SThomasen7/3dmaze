#ifndef _CAMERA_MOVEMENT_QUEUE_COMPONENT_H_
#define _CAMERA_MOVEMENT_QUEUE_COMPONENT_H_ 1

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

struct CameraMovementQueueComponent{
  std::vector<glm::vec3> movements;
  std::vector<glm::vec2> pans;
};


#endif
