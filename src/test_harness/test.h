#ifndef _TEST_H_
#define _TEST_H_ 1

#include <string>
#include <functional>

#define TEST_DIR std::string("test_output/")

typedef struct TestKit{
  bool __local_result__;
  bool __test_result__;
  std::ostringstream& ERR;
  std::ostringstream& LOG;
} TestKit;

class Test{
  
private:
  std::function<void(TestKit&)> test;

  std::string name;
  std::string error;
  std::string log;

  bool status;

public:

  Test(std::string name, std::function<void(TestKit&)> test);
  Test(const Test& other);

  void run();

  std::string error_output();
  std::string log_output();

  bool get_status();
  std::string get_name();
  void operator=(const Test& test);

};


#endif
