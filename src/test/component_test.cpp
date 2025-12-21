#include "component_test.h"
#include <iostream>
#include <memory>

MAKE_TEST(ComponentPoolTest){
  TEST_START();

  ComponentPool<int> pool;
  Entity entity1;
  Entity entity2;

  CHECK_FALSE(pool.contains(entity1));
  CHECK_FALSE(pool.contains(entity1));
  CHECK_FALSE(pool.contains(entity2));

  pool.add(entity1, 5);
  CHECK_TRUE(pool.contains(entity1));
  CHECK_FALSE(pool.contains(entity2));
  CHECK_EQ(pool.get(entity1), 5)
  CHECK_EQ(pool[entity1], 5)

  pool[entity2] = 10;
  CHECK_TRUE(pool.contains(entity1));
  CHECK_TRUE(pool.contains(entity2));
  CHECK_EQ(pool.get(entity1), 5)
  CHECK_EQ(pool[entity1], 5)
  CHECK_EQ(pool.get(entity2), 10)
  CHECK_EQ(pool[entity2], 10)

  pool.erase(entity1);
  CHECK_FALSE(pool.contains(entity1));
  CHECK_TRUE(pool.contains(entity2));
  CHECK_EQ(pool.get(entity2), 10)
  CHECK_EQ(pool[entity2], 10)

  pool.erase(entity2);
  CHECK_FALSE(pool.contains(entity1));
  CHECK_FALSE(pool.contains(entity2));

}

void set_component_tests(){
  TEST(ComponentTestSuite, ComponentPoolTest);
}
