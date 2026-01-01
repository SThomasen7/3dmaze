#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_ 1

#include "system.h"
#include "scene.h"

class RenderSystem : private System{
  // Inherit dispatcher reference from system

public:

  RenderSystem();

  void init(EventDispatcher* dispatcher) override;
  void process(Scene& scene, float dt) override;
  void shutdown() override;

  void preLoadScene(Scene& scene);
  void clearScene(Scene& scene);
  void loadSceneLights(Scene& scene);

};

#endif

