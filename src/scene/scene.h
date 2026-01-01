#ifndef _SCENE_H_
#define _SCENE_H_ 1

#include "entity_manager.h"
#include "event_dispatcher.h"
#include "system_events.h"

class Scene{

public:

  Scene();

  EntityManager& getEntityManager();
  void clear();
  void setEventDispatcher(EventDispatcher* dispatcher);

  int active_camera;

private:

  void handleScreenResizeEvent(const ResizeScreenEvent &event);

  EntityManager entity_manager;
  EventDispatcher* dispatcher;

};


#endif
