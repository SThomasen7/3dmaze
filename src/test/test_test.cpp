#include "test_test.h"

MAKE_TEST(TestTestPass){
  TEST_START();
  LOG << "Testing TestTest log" << std::endl;
  CHECK_EQ(0.4, 0.4);
}

MAKE_TEST(TestTestFail){
  TEST_START();
  LOG << "Testing TestTestFail log" << std::endl;
  CHECK_NEQ(0.4, 18);
}

MAKE_TEST(TestTestFail2_123456789012345678901234567890){
  TEST_START();
  CHECK_NEQ(0.6, 25);
  LOG << "Testing TestTestFail2 log" << std::endl;
}


void set_test_tests(){
  TEST(TestTestSuite, TestTestPass);
  TEST(TestTestSuite, TestTestFail);
  TEST(TestTestSuite2, TestTestPass);
  TEST(TestTestSuite2, TestTestPass);
  TEST(TestTestSuite2, TestTestFail2_123456789012345678901234567890);
}
