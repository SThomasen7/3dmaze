#ifndef _WINDOW_MANAGER_H_
#define _WINDOW_MANAGER_H_ 1

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include "application_settings.h"


class WindowManager{

  GLFWwindow* window;

public:

  WindowManager() : window(nullptr) {}

  void init();
  void shutdown();

  void pollEvents(ApplicationSettings& settings);

  void renderWidgets();
  void preRender();
  void postRender();
  void bufferResize(int width, int height);
  //GLFWwindow* getWindow();

private:
  //void handleScreenResizeEvent(const ResizeScreenEvent &event);
};

#endif
