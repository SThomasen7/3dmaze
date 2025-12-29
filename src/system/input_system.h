#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_ 1

#include "system.h"
#include "application_settings.h"
#include "window_manager.h"
#include <GL/glew.h> 
#include <GLFW/glfw3.h>


class InputSystem : public System{

public:
  InputSystem() { window_manager = nullptr; entity_manager = nullptr; };

  void init() override;
  void process(Scene& scene, float dt) override;
  void shutdown() override;

  void pollEvents(ApplicationSettings& settings);
  void setupWindow(WindowManager& window_manager);
  void setupEntityManager(EntityManager& entity_manager);

private:
  WindowManager* window_manager;
  EntityManager* entity_manager;
  double mouse_xpos, mouse_ypos;

  static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

};

#endif
