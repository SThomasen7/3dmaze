#ifndef _EVENT_DISPATCHER_H_
#define _EVENT_DISPATCHER_H_ 1

#include <vector>
#include <functional>
#include <unordered_map>
#include <typeindex>

#include "event.h"

class EventDispatcher{

private:
  std::unordered_map<
    std::type_index, 
    std::vector<std::function<void(IEvent*)>>
  > _listeners;

public:

  template<typename EventType>
  void subscribe(std::function<void(const EventType&)> listener){
    // Get the listener for this type of event
    auto &listeners = _listeners[typeid(EventType)];

    // Push back this listener in the list of listeners
    listeners.push_back([listener](IEvent* event) {
        listener(*static_cast<EventType*>(event));
    });
  }

  // Dispatch an event of type EventType to all of the listeners listening on this event
  template <typename EventType>
  void dispatch(EventType& event) {
    auto& listeners = _listeners[typeid(EventType)];
    for (auto& listener : listeners) {
      listener(&event);
    }
  }

  void clear(){
    _listeners.clear();
  }

  template <typename EventType>
  void clear(EventType& event){
    auto& listeners = _listeners[typeid(EventType)];
    listeners.clear();
  }

};

#endif
