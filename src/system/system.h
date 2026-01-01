#ifndef _SYSTEM_H_
#define _SYSTEM_H_ 1

#include "scene.h"
#include "event_dispatcher.h"

class System{

public:
  System() {}

  virtual void init(EventDispatcher* _dispatcher) = 0;
  virtual void process(Scene& scene, float dt) = 0;
  virtual void shutdown() = 0;

protected:
  EventDispatcher* dispatcher;

};

#endif
