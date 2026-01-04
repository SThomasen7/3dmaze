#include "input_system.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

InputSystem::InputSystem(){ 
  window_manager = nullptr; 
  entity_manager = nullptr; 
  dispatcher = nullptr;
  pause_delay = glfwGetTime();
  f1_delay = glfwGetTime();
}

void InputSystem::init(EventDispatcher* dispatcher){
  this->dispatcher = dispatcher;
  LOG(LL::Info, "Initializing Input System.");
  glfwSetWindowUserPointer(window_manager->getWindow(), this);
  holdCursor();
  glfwSetKeyCallback(window_manager->getWindow(), InputSystem::keyCallback);
  glfwGetCursorPos(window_manager->getWindow(), &mouse_xpos, &mouse_ypos);
  glfwSetCursorPosCallback(window_manager->getWindow(), InputSystem::mouseCallback);
  glfwSetFramebufferSizeCallback(window_manager->getWindow(), 
      InputSystem::framebufferSizeCallback);
  first_mouse = true;
  is_full_screen = false;
}

void InputSystem::process(Scene& scene, float dt){
  float move_speed = 4.0f * dt;
  GLFWwindow* window = window_manager->getWindow();
  EntityManager& entity_manager = scene.getEntityManager();

  if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    move_speed *= 2.0f;
  }

  glm::vec3 move(0.0f);

  // Calculate the movement direction
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    move += glm::vec3(0.0f, 0.0f, 1.0f);
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    move += glm::vec3(0.0f, 0.0f, -1.0f);
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    move += glm::vec3(-1.0f, 0.0f, 0.0f);
  }
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    move += glm::vec3(1.0f, 0.0f, 0.0f);
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

void InputSystem::setupAppSettings(ApplicationSettings& settings){
  this->settings = &settings;
}

void InputSystem::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  auto* self = static_cast<InputSystem*>(
        glfwGetWindowUserPointer(window)
  );

  if(key == GLFW_KEY_Q){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if(key == GLFW_KEY_ESCAPE){
    float current_time = glfwGetTime();
    if(current_time - self->pause_delay < 0.4f){
      return;
    }
    self->pause_delay = current_time;
    TogglePauseEvent pause_event;
    auto* self = static_cast<InputSystem*>(
          glfwGetWindowUserPointer(window)
    );
    self->dispatcher->dispatch(pause_event);
  }

  if(key == GLFW_KEY_F1){

    float current_time = glfwGetTime();
    if(current_time - self->f1_delay < 0.4f){
      return;
    }
    self->f1_delay = current_time;

    LOG(LL::Verbose, "F1 pressed");
    if(self->is_full_screen){
      self->makeWindowed();
    }
    else{
      self->makeFullScreen();
    }
    
  }
}

void InputSystem::framebufferSizeCallback(GLFWwindow* window, int width, int height){
  LOG(LL::Verbose, "Frame buffer resize callback: ", width, " ", height);
  ResizeScreenEvent resize_event(width, height);
  auto* self = static_cast<InputSystem*>(
        glfwGetWindowUserPointer(window)
  );
  self->dispatcher->dispatch(resize_event);
  self->window_manager->bufferResize(width, height);
}


void InputSystem::mouseCallback(GLFWwindow* window, double temp_mousex, double temp_mousey){

  float mouse_speed = 0.5f;
  auto* self = static_cast<InputSystem*>(
        glfwGetWindowUserPointer(window)
  );

  if(self->settings->is_paused){
    return;
  }

  if(self->first_mouse){
    self->mouse_xpos = temp_mousex; 
    self->mouse_ypos = temp_mousey; 
    self->first_mouse = false;
    return;
  }
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

void InputSystem::releaseCursor(){
  glfwSetInputMode(window_manager->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // Move mouse to the center of the screen
  int width, height;
  first_mouse = true;
  glfwGetWindowSize(window_manager->getWindow(), &width, &height);
  double centerX = width / 2.0;
  double centerY = height / 2.0;
  glfwSetCursorPos(window_manager->getWindow(), centerX, centerY);
  first_mouse = true;
}

void InputSystem::holdCursor(){
  glfwSetInputMode(window_manager->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  first_mouse = true;
}

void InputSystem::makeFullScreen(){
  if(is_full_screen){
    return;
  }
  is_full_screen = true;
  window_manager->makeFullScreen();
}

void InputSystem::makeWindowed(){
  if(!is_full_screen){
    return;
  }
  is_full_screen = false;
  window_manager->makeWindowed();
}

