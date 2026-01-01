#ifndef _ENGINE_H_
#define _ENGINE_H_ 1

#include "opengl_headers.h"
#include "window_manager.h"
#include "render_system.h"
#include "input_system.h"
#include "physics_system.h"
#include "application_settings.h"
#include "event_dispatcher.h"
#include "system_events.h"
#include "scene.h"
#include <string>

class Engine{

public:

  Engine();
  void init();
  void shutdown();

  // game loop
  void loadScene(std::string filename);
  void execute();

private:

  void handlePauseEvent(const TogglePauseEvent& event);

  // Managers
  WindowManager window_manager;

  Scene scene;

  // Systems -- only these should touch the scene
  RenderSystem render_system;
  InputSystem input_system;
  PhysicsSystem physics_system;

  // Settings
  ApplicationSettings settings;

  // Event Dispatcher
  EventDispatcher event_dispatcher;

};

#endif
