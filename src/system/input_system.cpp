#include "input_system.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void InputSystem::init(){
  LOG(LL::Info, "Initializing Input System.");
  glfwSetKeyCallback(window_manager.getWindow(), key_callback);
}

void InputSystem::process(Scene& scene){

}

void InputSystem::shutdown(){
  LOG(LL::Info, "Shutting down Input System.");
}

void InputSystem::pollEvents(ApplicationSettings& settings){
  settings.should_close = settings.should_close || window_should_close;
}

void InputSystem::setupWindow(WindowManager& window_manager){
  this->window_manager = window_manager;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(key == GLFW_KEY_Q){
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}
