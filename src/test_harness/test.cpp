#include "test.h"
#include <filesystem>
#include <fstream>
#include <iostream>

using std::string;
using std::function;
using std::ostream;
using std::ostringstream;
//using std::ofstream;
using std::endl;
using std::cout;
//using std::cerr;

Test::Test(string name, function<void(TestKit&)> test){
  this->test = test;
  this->name = name;
  status = false;
}

void Test::run(){
  ostringstream log_stream;
  ostringstream error_stream;
  TestKit kit{ false, true, log_stream, error_stream};
  test(kit);
  status = kit.__test_result__;
  log += kit.LOG.str();
  error += kit.ERR.str();
}

string Test::log_output(){
  return log;
}

string Test::error_output(){
  return error;
}

bool Test::get_status(){
  return status;
}

string Test::get_name(){
  return name;
}


void Test::operator=(const Test& test){
  this->test = test.test;
  this->name = test.name;
  this->error = test.error;
  this->log = test.log;
  this->status = test.status;
}

Test::Test(const Test& other){
  this->test = other.test;
  this->name = other.name;
  this->error = other.error;
  this->log = other.log;
  this->status = other.status;
}

