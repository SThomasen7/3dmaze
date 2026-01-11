#ifndef _SCENE_H_
#define _SCENE_H_ 1

#include "entity_manager.h"
#include "event_dispatcher.h"
#include "system_events.h"
#include "resource_manager.h"

#include "shader_component.h"

#include <memory>

class Scene{

public:

  Scene();

  EntityManager& getEntityManager();
  void clear();
  void setEventDispatcher(EventDispatcher* dispatcher);

  int active_camera;
  Entity* default_texture;

  ResourceManager<std::shared_ptr<ShaderComponentData>> shader_manager;
  ResourceManager<std::shared_ptr<MeshComponentData>> mesh_manager;
  ResourceManager<std::shared_ptr<TextureComponentData>> texture_manager;

private:

  void handleScreenResizeEvent(const ResizeScreenEvent &event);

  EntityManager entity_manager;
  EventDispatcher* dispatcher;

};


#endif
