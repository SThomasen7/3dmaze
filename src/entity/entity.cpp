#include "entity.h"
#include "entity_allocator.h"

Entity::Entity() : id(EntityAllocator::getNewId()){ }

Entity::Entity(const Entity& other) : id(other.id){ }
