#include "input_system.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void InputSystem::init(){
  LOG(LL::Info, "Initializing Input System.");
  glfwSetWindowUserPointer(window_manager->getWindow(), this);
  glfwSetKeyCallback(window_manager->getWindow(), key_callback);
  glfwGetCursorPos(window_manager->getWindow(), &mouse_xpos, &mouse_ypos);
  glfwSetInputMode(window_manager->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window_manager->getWindow(), InputSystem::mouseCallback);
}

void InputSystem::process(Scene& scene, float dt){
  float move_speed = 5.0f * dt;
  float timestep = 1.0f/60.0f; // framerate TODO make dynamic
  GLFWwindow* window = window_manager->getWindow();
  EntityManager& entity_manager = scene.getEntityManager();

  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    move_speed *= 2.0f;
  }

  glm::vec3 move(0.0f);

  // Calculate the movement direction
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    move += glm::vec3(1.0f, 0.0f, 0.0f);
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    move += glm::vec3(-1.0f, 0.0f, 0.0f);
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    move += glm::vec3(0.0f, 0.0f, -1.0f);
  }
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    move += glm::vec3(0.0f, 0.0f, 1.0f);
  }
  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
    move += glm::vec3(0.0f, 1.0f, 0.0f);
  }
  if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
    move += glm::vec3(0.0f, -1.0f, 0.0f);
  }

  using EntityView = EntityManager::EntityView;
  EntityView* eview = entity_manager.createEntityView<
      CameraComponent, 
      CameraMovementQueueComponent
    >();

  if(move != glm::vec3(0.0f)){
    move = glm::normalize(move)*move_speed;
  }

  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    
    if(move == glm::vec3(0.0f)){
      continue;
    }

    CameraMovementQueueComponent& component = entity_manager.getCameraMovementQueue(*entity_ptr);
    component.movements.push_back(move);
    component.pans.push_back(glm::vec2(0.0f));

  }
  entity_manager.destroyEntityView(&eview);
}

void InputSystem::shutdown(){
  LOG(LL::Info, "Shutting down Input System.");
  window_manager = nullptr;
  entity_manager = nullptr;
}

void InputSystem::pollEvents(ApplicationSettings& settings){

}

void InputSystem::setupWindow(WindowManager& window_manager){
  this->window_manager = &window_manager;
}

void InputSystem::setupEntityManager(EntityManager& entity_manager){
  this->entity_manager = &entity_manager;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(key == GLFW_KEY_Q){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void InputSystem::mouseCallback(GLFWwindow* window, double temp_mousex, double temp_mousey){

  float mouse_speed = 0.5f;
  auto* self = static_cast<InputSystem*>(
        glfwGetWindowUserPointer(window)
  );
  using EntityView = EntityManager::EntityView;
  EntityView* eview = self->entity_manager->createEntityView<
      CameraComponent, 
      CameraMovementQueueComponent
    >();

  // Handle mouse motion
  //glfwGetCursorPos(window, &temp_mousex, &temp_mousey);
  double delta_x, delta_y;
  delta_x = -(self->mouse_xpos - temp_mousex);
  delta_y = self->mouse_ypos - temp_mousey;

  if(abs(delta_x) < 0.0001){
    delta_x = 0.0;
  }
  if(abs(delta_y) < 0.0001){
    delta_y = 0.0;
  }

  if(delta_x == 0.0 && delta_y == 0.0){
    return;
  }

  self->mouse_xpos = temp_mousex;
  self->mouse_ypos = temp_mousey;

  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    
    CameraMovementQueueComponent& component = self->entity_manager->getCameraMovementQueue(*entity_ptr);
    component.movements.push_back(glm::vec3(0.0f));
    component.pans.push_back(glm::vec2(delta_x, delta_y)*mouse_speed);
  }
}
