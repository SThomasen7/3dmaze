#include "physics_system.h"
#include <glm/vec2.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi


void PhysicsSystem::init(){
  LOG(LL::Info, "Initializing Physics System.");
}

void PhysicsSystem::process(Scene& scene){

  resolve_camera_movements(scene);

}

void PhysicsSystem::shutdown(){
  LOG(LL::Info, "Shutting down Physics System.");
}

void PhysicsSystem::resolve_camera_movements(Scene& scene){
  // Resolve camera movements, in the future check for bounding box collisions
  EntityManager& entity_manager = scene.getEntityManager();
  using EntityView = EntityManager::EntityView;
  EntityView* eview = entity_manager.createEntityView<
      CameraComponent, 
      CameraMovementQueueComponent
    >();

  glm::vec3 resolved_position(0.0f);
  glm::vec2 resolved_pan(0.0f);
  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    
    CameraMovementQueueComponent& component = entity_manager.getCameraMovementQueue(*entity_ptr);
    for(size_t i = 0; i < component.movements.size(); i++){
      resolved_position += component.movements[i];
      resolved_pan += component.pans[i];
    }

    // Clear out the requested movements
    component.movements.clear();
    component.pans.clear();

    // update the position component
    PositionComponent& pos_component = entity_manager.getPosition(*entity_ptr);
    LOG(LL::Verbose, "Moving camera delta: (", resolved_position.x, " ", resolved_position.y,
        " ", resolved_position.z, ")");
    pos_component.position += resolved_position;
    LOG(LL::Verbose, "Moving camera new position: (", pos_component.position.x, 
        " ", pos_component.position.y, " ", pos_component.position.z, ")");

    // Update the up, lookat, and right vectors
    CameraComponent& camera_component = entity_manager.getCamera(*entity_ptr);
    LOG(LL::Verbose, "panning camera delta: (", resolved_pan.x, " ", resolved_pan.y, ")");
    rotate_camera_x_y(camera_component, resolved_pan.x, resolved_pan.y);

  }
  entity_manager.destroyEntityView(&eview);
}

void PhysicsSystem::rotate_camera(CameraComponent& camera, const glm::vec3 &axis, const float angle){
  glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::normalize(axis));
  camera.lookat = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(camera.lookat, 1.0f)));
  camera.right = glm::cross(camera.lookat, camera.up);
}


void PhysicsSystem::rotate_camera_x_y(CameraComponent& camera, float x, float y){
  if(x != 0.0f){
    //rotate(up, x*(1-std::abs(glm::dot(lookat, up))));
    rotate_camera(camera, camera.up, x);
  }
  if(y != 0.0f){
    if(!(y > 0 && glm::dot(camera.lookat, camera.up) > 0.95) && 
        !(y < 0 && glm::dot(camera.lookat, camera.up) < -0.95)){
      rotate_camera(camera, camera.right, y);
    }
  }
}

