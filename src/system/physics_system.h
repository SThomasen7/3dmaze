#ifndef _PHYSICS_SYSTEM_H_
#define _PHYSICS_SYSTEM_H_ 1

#include "system.h"
#include "scene.h"
#include "camera_component.h"
#include <glm/vec3.hpp>

class PhysicsSystem : public System{

public:
  PhysicsSystem() { }

  void init() override;
  void process(Scene& scene) override;
  void shutdown() override;

private:

  void resolve_camera_movements(Scene& scene);
  void rotate_camera(CameraComponent& camera, const glm::vec3 &axis, const float angle);
  void rotate_camera_x_y(CameraComponent& camera, float x, float y);

};

#endif
