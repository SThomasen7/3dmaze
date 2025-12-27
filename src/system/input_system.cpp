#include "input_system.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void InputSystem::init(){
  LOG(LL::Info, "Initializing Input System.");
  glfwSetKeyCallback(window_manager->getWindow(), key_callback);
  glfwGetCursorPos(window_manager->getWindow(), &mouse_xpos, &mouse_ypos);
  glfwSetInputMode(window_manager->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputSystem::process(Scene& scene){
  float speed = 0.3f;
  float timestep = 1.0f/60.0f; // framerate TODO make dynamic
  GLFWwindow* window = window_manager->getWindow();
  EntityManager& entity_manager = scene.getEntityManager();

  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    speed *= 2.0f;
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

  // Handle mouse motion
  double temp_mousex, temp_mousey;
  glfwGetCursorPos(window, &temp_mousex, &temp_mousey);
  double delta_x, delta_y ;
  delta_x = mouse_xpos - temp_mousex;
  delta_y = mouse_ypos - temp_mousey;
  mouse_xpos = temp_mousex;
  mouse_ypos = temp_mousey;
  using EntityView = EntityManager::EntityView;
  EntityView* eview = entity_manager.createEntityView<
      CameraComponent, 
      CameraMovementQueueComponent
    >();

  glm::vec3 resolved_position(0.0f);
  glm::vec2 resolved_pan(0.0f);
  for(auto entity_ptr = eview->begin(); entity_ptr != eview->end(); entity_ptr++){
    
    if(move == glm::vec3(0.0f) && delta_x == 0.0 && delta_y == 0.0){
      continue;
    }

    CameraMovementQueueComponent& component = entity_manager.getCameraMovementQueue(*entity_ptr);
    if(move != glm::vec3(0.0f)){
      move = glm::normalize(move)*speed;
    }
    component.movements.push_back(move);
    component.pans.push_back(glm::vec2(delta_x, delta_y));

  }
  entity_manager.destroyEntityView(&eview);
}

void InputSystem::shutdown(){
  LOG(LL::Info, "Shutting down Input System.");
}

void InputSystem::pollEvents(ApplicationSettings& settings){

}

void InputSystem::setupWindow(WindowManager& window_manager){
  this->window_manager = &window_manager;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(key == GLFW_KEY_Q){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}
