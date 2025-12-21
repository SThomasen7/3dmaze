#ifndef _TEST_HARNESS_H_
#define _TEST_HARNESS_H_ 1

#include "test_suite.h"
#include "test_macros.h"

#include <map>
#include <string>
#include <functional>
#include <ostream>

class TestHarness{

public:
  static std::map<std::string, TestSuite> test_suites;

  TestHarness() {};

  static void run_all_tests(){
    for(auto& [key, value] : test_suites){
      value.prepare_test_env();
      value.execute_tests();
      value.report_results();
    }
  }

  static void create_test(std::string test_suite, 
                std::string test_name,
                std::function<void(TestKit&)> test){

    // Create the test suite if it already exists
    if(test_suites.find(test_suite) == test_suites.end()){
      test_suites[test_suite] = TestSuite(test_suite);
    }
    
    Test test_obj(test_name,test);
    test_suites[test_suite].tests.push_back(test_obj);
  }

};

#endif
