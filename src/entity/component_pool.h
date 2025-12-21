#ifndef _COMPONENT_POOL_H_
#define _COMPONENT_POOL_H_ 1

#include <cstdint>
//#include <map>
#include <vector>
#include "entity.h"

template <typename T>
class ComponentPool{

public:
  ComponentPool(){ }

  inline void add(Entity entity, T component){
    entity_map[entity.id] = component;
  }

  inline void erase(Entity entity){
    entity_map.erase(entity.id);
  }

  inline T get(Entity entity){
    return entity_map[entity.id];
  }

  inline bool contains(Entity entity){
    return entity_map.find(entity.id) != entity_map.end();
  }

  // bracket operator
  T operator [](Entity entity) const {
    return entity_map[entity.id];
  }

  T & operator [](Entity entity) {
    return entity_map[entity.id];
  }


private:

  std::map<uint32_t, T> entity_map;

};

#endif 
