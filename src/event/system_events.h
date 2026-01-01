#ifndef _SYSTEM_EVENTS_H_
#define _SYSTEM_EVENTS_H_

#include "event.h"

struct ResizeScreenEvent : IEvent {
public:

  ResizeScreenEvent(float x, float y) : buffer_x(x), buffer_y(y){}

  float buffer_x, buffer_y;
};


#endif
