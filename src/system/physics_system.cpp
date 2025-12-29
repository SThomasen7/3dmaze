#include "physics_system.h"
#include <glm/vec2.hpp>
#include <glm/ext/matrix_transform.hpp> // glm::translate
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi


void PhysicsSystem::init(){
  LOG(LL::Info, "Initializing Physics System.");
}

void PhysicsSystem::process(Scene& scene, float dt){

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

  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    
    CameraMovementQueueComponent& component = entity_manager.getCameraMovementQueue(*entity_ptr);

    if(component.movements.size() == 0){
      continue;
    }

    for(size_t i = 0; i < component.movements.size(); i++){
      CameraComponent& camera_component = entity_manager.getCamera(*entity_ptr);
      PositionComponent& pos_component = entity_manager.getPosition(*entity_ptr);

      rotate_camera_x_y(camera_component, component.pans[i].x, 
          component.pans[i].y);
      LOG(LL::Verbose, "Moving camera new position: (", pos_component.position.x, 
       " ", pos_component.position.y, " ", pos_component.position.z, ")");

      // Update the up, lookat, and right vectors
      LOG(LL::Verbose, "panning camera delta: (", component.pans[i].x, " ", 
          component.pans[i].y, ")");
      LOG(LL::Verbose, "lookat (", camera_component.lookat.x, " ", 
          camera_component.lookat.y, " ", camera_component.lookat.z, ")");
      glm::vec3 displace = glm::mat3(camera_component.lookat, camera_component.up,
            camera_component.right) * component.movements[i];
      pos_component.position += displace;
      LOG(LL::Verbose, "Moving camera delta: (", displace.x, " ", displace.y,
          " ", displace.z, ")");
    }

    // Clear out the requested movements
    component.movements.clear();
    component.pans.clear();
  }
  entity_manager.destroyEntityView(&eview);
}

void PhysicsSystem::rotate_camera_x_y(CameraComponent& camera, float x, float y){
  x += camera.yaw;
  y += camera.pitch;
  if(y > 89.0f){
    y = 89.0f;
  }
  if(y < -89.0f){
    y = -89.0f;
  }

  glm::vec2 smoothed = glm::mix(glm::vec2(camera.yaw, camera.pitch),
      glm::vec2(x, y), 0.25f);

  x = smoothed.x;
  y = smoothed.y;
  camera.yaw = x;
  camera.pitch = y;
  LOG(LL::Verbose, "Pitch, yaw (2): ", camera.pitch, " ", camera.yaw);
  x = glm::radians(x);
  y = glm::radians(y);

  camera.lookat.x = cos(y) * cos(x);
  camera.lookat.y = sin(y);
  camera.lookat.z = cos(y) * sin(x);
  camera.lookat = glm::normalize(camera.lookat);
  camera.right = glm::cross(camera.lookat, camera.up);
}

