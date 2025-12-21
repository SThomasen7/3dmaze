#include "entity_test.h"
#include "entity_allocator.h"
#include <iostream>
#include <memory>

MAKE_TEST(InitTest){
  TEST_START();
  LOG << "Testing incremental IDs for entities" << std::endl;

  std::unique_ptr<Entity> entity = std::make_unique<Entity>();
  uint32_t first_id = entity->id;
  CHECK_EQ(entity->id, first_id);
  entity = std::make_unique<Entity>();
  CHECK_EQ(entity->id, first_id+1);
  entity = std::make_unique<Entity>();
  CHECK_EQ(entity->id, first_id+2);
  entity = std::make_unique<Entity>();
  CHECK_EQ(entity->id, first_id+3);

  Entity entity2;
  CHECK_EQ(entity2.id, first_id+4);
  Entity entity3(entity2);
  CHECK_EQ(entity2.id, first_id+4);
  entity = std::make_unique<Entity>();
  CHECK_EQ(entity->id, first_id+5);
}

void set_entity_tests(){
  TEST(EntityTestSuite, InitTest);
}
