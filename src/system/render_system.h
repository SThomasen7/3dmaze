#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_ 1

#include "system.h"
#include "scene.h"

class RenderSystem : public System{

public:

  RenderSystem();

  void init() override;
  void process(Scene& scene, float dt) override;
  void shutdown() override;

  void preLoadScene(Scene& scene);
  void clearScene(Scene& scene);
  void loadSceneLights(Scene& scene);

};

#endif

