#ifndef _ENTITY_H_
#define _ENTITY_H_ 1

#include <cstdint>

class Entity{

public:

  const uint32_t id;

  Entity();

  Entity(const Entity& other);

};

#endif 
