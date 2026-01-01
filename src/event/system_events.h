#ifndef _SYSTEM_EVENTS_H_
#define _SYSTEM_EVENTS_H_

#include "event.h"

struct ResizeScreenEvent : IEvent {
public:

  ResizeScreenEvent(float x, float y) : buffer_x(x), buffer_y(y){}

  float buffer_x, buffer_y;
};

struct TogglePauseEvent : IEvent {
public:

  TogglePauseEvent(bool force, bool paused) : force(force), pause(paused) {}
  TogglePauseEvent() : force(false), pause(false) {}

  bool force, pause;
};

#endif
