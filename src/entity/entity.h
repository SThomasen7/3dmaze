#ifndef _ENTITY_H_
#define _ENTITY_H_ 1

#include <cstdint>

class EntityManager;

class Entity{

public:

  const uint32_t id;
  Entity(const Entity& other);

private:

  friend EntityManager;
  Entity();

};

bool operator==(const Entity& lhs, const Entity& rhs);
bool operator!=(const Entity& lhs, const Entity& rhs);

#endif 
