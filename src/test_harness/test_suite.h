#ifndef _TEST_SUITE_H_
#define _TEST_SUITE_H_ 1

#include "test.h"

#include <string>
#include <vector>
#include <filesystem>

class TestSuite{

  public:
  std::vector<Test> tests;
  std::string name;
  size_t exec_fail_count;

  std::string directory;

  public:

  TestSuite(std::string name);
  TestSuite(const TestSuite& other);
  TestSuite(){};
  void prepare_test_env();
  void execute_tests();
  void report_results();
  void add_test(Test test);

  std::string get_name();

  void operator=(const TestSuite& test);
};

#endif
