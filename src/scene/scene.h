#ifndef _SCENE_H_
#define _SCENE_H_ 1

#include "entity_manager.h"
#include "event_dispatcher.h"
#include "system_events.h"
#include "resource_manager.h"

#include "shader_component.h"

class Scene{

public:

  Scene();

  EntityManager& getEntityManager();
  void clear();
  void setEventDispatcher(EventDispatcher* dispatcher);

  int active_camera;

  ResourceManager<ShaderComponentData> shader_manager;
  ResourceManager<MeshComponentData> mesh_manager;
  ResourceManager<TextureComponentData> texture_manager;

private:

  void handleScreenResizeEvent(const ResizeScreenEvent &event);

  EntityManager entity_manager;
  EventDispatcher* dispatcher;

};


#endif
