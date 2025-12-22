#include "component_test.h"
#include "entity_manager.h"
#include <iostream>
#include <memory>


MAKE_TEST(ComponentPoolTest){
  EntityManager entity_manager;
  TEST_START();

  ComponentPool<int> pool;
  Entity entity1 = entity_manager.createEntity();
  Entity entity2 = entity_manager.createEntity();

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
