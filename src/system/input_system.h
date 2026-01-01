#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_ 1

#include "system.h"
#include "application_settings.h"
#include "window_manager.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>


class InputSystem : public System{
  // Inherit dispatcher reference from system

public:
  InputSystem();

  void init(EventDispatcher* dispatcher) override;
  void process(Scene& scene, float dt) override;
  void shutdown() override;

  void pollEvents(ApplicationSettings& settings);
  void setupWindow(WindowManager& window_manager);
  void setupEntityManager(EntityManager& entity_manager);
  void setupAppSettings(ApplicationSettings& settings);

  void releaseCursor();
  void holdCursor();

private:
  WindowManager* window_manager;
  EntityManager* entity_manager;
  ApplicationSettings* settings;
  double mouse_xpos, mouse_ypos;
  bool first_mouse;
  float pause_delay;

  static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
  static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

};

#endif
