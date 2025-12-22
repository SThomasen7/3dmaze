#ifndef _SCENE_LOADER_H_
#define _SCENE_LOADER_H_ 1

#include "scene.h"
#include "entity_manager.h"
#include <string>

class SceneLoader{

public:
  static void load(Scene& scene, std::string filename);
  static void free(Scene& scene, std::string filename);

private:
  SceneLoader() {};

};

#endif
