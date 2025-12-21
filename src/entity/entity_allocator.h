#ifndef _ENTITY_ALLOCATOR_H_
#define _ENTITY_ALLOCATOR_H_ 1

#include <cstdint> 

class EntityAllocator{

private:
  static uint32_t current_id;

public: 

  inline static uint32_t getNewId(){
    uint32_t id = current_id;
    current_id += 1;
    return id;
  }

};

#endif
