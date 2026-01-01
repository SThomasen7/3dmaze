#include "window_manager.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stdexcept>
#include "logger.h"
//#include "event_dispatcher.h"

void WindowManager::init(){
  // Create a GLFW windowed mode window and its OpenGL context
  if (!glfwInit()) {
    LOG(LL::Error, "Failed to initialize GLFW.");
    throw std::runtime_error("Failed to intialize GLFW.");
  }

  window = glfwCreateWindow(1280, 720, "ImGui OpenGL Example", nullptr, nullptr);
  if (!window) {
    LOG(LL::Error, "Could not create GLFW window!");
    throw std::runtime_error("Could not create GLFW window.");
  }

  bufferResize(1280, 720);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Make the OpenGL context current
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  // Enable vsync
 
  // Initialize GLEW (after making the OpenGL context current)
  glewExperimental = GL_TRUE;  // Enable modern OpenGL features
  if (glewInit() != GLEW_OK) {
    LOG(LL::Error, "Failed to initialize GLEW.");
    throw std::runtime_error("Failed to intialize GLEW.");
  }

  // ImGUI calls
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  // Setup ImGui platform bindings (GLFW) and renderer bindings (OpenGL)
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");  // You may need to specify your OpenGL version
  LOG(LL::Info, "Window manager initialized.");

  // Register events
  //EVENT_DISPATCHER.subscribe<ResizeScreenEvent>(
      //std::bind(&WindowManager::handleScreenResizeEvent, this, std::placeholders::_1));

}

void WindowManager::shutdown(){
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  if(window){
    glfwDestroyWindow(window);
    window = nullptr;
  }
  glfwTerminate();
  LOG(LL::Info, "Window manager shutdown.");
}

void WindowManager::pollEvents(ApplicationSettings& settings){
  // poll events
  glfwPollEvents();

  if(glfwWindowShouldClose(window)){
    settings.should_close = true;
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void WindowManager::renderWidgets(){
  // None at the moment
}

void WindowManager::preRender(){
  ImGui::Render();
}

void WindowManager::postRender(){
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  // Swap buffers
  glfwSwapBuffers(window);
}

void WindowManager::bufferResize(int width, int height){
  glViewport(0, 0, width, height);
  buffer_width = width;
  buffer_height = height;
}

GLFWwindow* WindowManager::getWindow(){
  return window;
}

void WindowManager::makeFullScreen(){
  LOG(LL::Verbose, "Making full screen...");
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}

void WindowManager::makeWindowed(){
  LOG(LL::Verbose, "Making windowed..");
  glfwSetWindowMonitor(window, NULL, 0, 0, 0, 0, GLFW_DONT_CARE);
  glfwSetWindowMonitor(window, NULL, buffer_width/2, buffer_height/2, 
      buffer_width, buffer_height, GLFW_DONT_CARE);
}

