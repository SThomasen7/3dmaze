#include "component_test.h"
#include "entity_manager.h"
#include <iostream>
#include <memory>

MAKE_TEST(ComponentPoolTest){
  TEST_START();

  ComponentPool<int> pool;
  Entity entity1 = ENTITY_MANAGER.createEntity();
  Entity entity2 = ENTITY_MANAGER.createEntity();

  CHECK_FALSE(pool.contains(entity1));
  CHECK_FALSE(pool.contains(entity1));
  CHECK_FALSE(pool.contains(entity2));

  pool.add(entity1, 5);
  CHECK_TRUE(pool.contains(entity1));
  CHECK_FALSE(pool.contains(entity2));
  CHECK_EQ(pool.at(entity1), 5)
  CHECK_EQ(pool[entity1], 5)

  pool[entity2] = 10;
  CHECK_TRUE(pool.contains(entity1));
  CHECK_TRUE(pool.contains(entity2));
  CHECK_EQ(pool.at(entity1), 5)
  CHECK_EQ(pool[entity1], 5)
  CHECK_EQ(pool.at(entity2), 10)
  CHECK_EQ(pool[entity2], 10)

  pool.erase(entity1);
  CHECK_FALSE(pool.contains(entity1));
  CHECK_TRUE(pool.contains(entity2));
  CHECK_EQ(pool.at(entity2), 10)
  CHECK_EQ(pool[entity2], 10)

  pool.erase(entity2);
  CHECK_FALSE(pool.contains(entity1));
  CHECK_FALSE(pool.contains(entity2));

}

void set_component_tests(){
  TEST(ComponentTestSuite, ComponentPoolTest);
}
