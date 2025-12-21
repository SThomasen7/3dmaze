#include "entity.h"
#include "entity_allocator.h"

Entity::Entity() : id(EntityAllocator::getNewId()){ }

Entity::Entity(const Entity& other) : id(other.id){ }

bool operator==(const Entity& lhs, const Entity& rhs){
  return lhs.id == rhs.id;
}

bool operator!=(const Entity& lhs, const Entity& rhs){
  return !(lhs == rhs);
}

