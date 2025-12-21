#include "test_harness.h"
#include "logger.h"
#include "test_test.h"
#include "entity_test.h"
#include "component_test.h"

#include <iostream>
int main(void){
  LOGGER.setLogLevel(LL::Disable);
  //set_test_tests();
  set_entity_tests();
  set_component_tests();
  RUN_ALL_TESTS();
  return 0; 
}

