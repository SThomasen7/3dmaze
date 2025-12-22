#ifndef _ENGINE_H_
#define _ENGINE_H_ 1

#include "opengl_headers.h"
#include "window_manager.h"
#include "render_system.h"
#include "application_settings.h"
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

  // Managers
  WindowManager window_manager;

  Scene scene;

  // Systems -- only these should touch the scene
  RenderSystem render_system;

  // Settings
  ApplicationSettings settings;

};

#endif
