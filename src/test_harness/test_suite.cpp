#include "test_suite.h"

#include <ostream>
#include <fstream>
#include <iostream>
#include <stdexcept>

using std::string;
using std::vector;
using std::ofstream;
using std::stringstream;
using std::cout;
using std::endl;

TestSuite::TestSuite(std::string name){
  this->name = name;
  exec_fail_count = 0;
}

void TestSuite::prepare_test_env(){
  // Build directory for test output
  directory = TEST_DIR+name+string("/");
  std::filesystem::create_directory(TEST_DIR);
  std::filesystem::create_directory(directory);
}

void TestSuite::execute_tests(){
  for(size_t i=0; i<tests.size(); i++){

    string test_name = tests[i].get_name();

    try{
      tests[i].run();

      // Write output
      ofstream log_file;
      log_file.open(directory+test_name+".log");
      log_file << tests[i].log_output(); 
      log_file.close();

      // Write error
      ofstream err_file;
      err_file.open(directory+test_name+".err");
      err_file << tests[i].error_output(); 
      err_file.close();

      // check if test failed
      if(!tests[i].get_status()){
        ofstream fail;
        fail.open(directory+test_name+".fail");
        fail.close();
      }
    }
    catch(const std::exception& e){
      std::cout << "Warning: Failed execution of " << tests[i].get_name() 
                << std::endl;
      std::cout << "Exception: " << e.what() << std::endl;
      ofstream exec_fail;
      exec_fail.open(directory+test_name+"_execution_fail");
      exec_fail.close();
      exec_fail_count++;
    }
  }
}

void TestSuite::add_test(Test test){
  this->tests.push_back(test);
}

void TestSuite::report_results(){
    ofstream test_results;
    test_results.open(directory+name+".out");
    //test_results << "========================================" << endl;
    //test_results << "  " << name << endl;
    //test_results << "========================================" << endl;
    test_results << "== " << name << " ";
    //cout << "========================================" << endl;
    //cout << "  " << name << endl;
    //cout << "========================================" << endl;
    cout << "== " << name << " ";

    size_t passed = 0;
    size_t failed = 0;
    // Print individual test results
    string individual_test_results = "";
    for(size_t i=0; i<tests.size(); i++){
      string result = tests[i].get_name().substr(0, 35);
      while(result.length() < 36){
        result += " ";
      }

      if(tests[i].get_status()){
        result += "PASS\n";
        passed += 1;
      }
      else{
        result += "FAIL\n";
        result += "***\n";
        result += tests[i].error_output();
        result += "\n";
        failed += 1;
      }
      individual_test_results += result;
    }

    test_results << "(" << passed << "/" << tests.size() << ") ";
    cout << "(" << passed << "/" << tests.size() << ") ";

    if(failed == 0 && exec_fail_count == 0){
      test_results << "OK\n";
      cout << "OK\n";
    }
    else{
      test_results << "FAIL\n";
      cout << "FAIL\n";
    }

    test_results << individual_test_results << endl;
    if(exec_fail_count > 0){
      test_results << "(" << exec_fail_count << " failed execution)" << '\n';
      cout << "(" << exec_fail_count << " failed execution)" << '\n';
    }
    cout << individual_test_results << endl;
    test_results.close();
}


string TestSuite::get_name(){
  return name;
}

void TestSuite::operator=(const TestSuite& test){
  this->tests = test.tests;
  this->name = test.name;
  this->exec_fail_count = test.exec_fail_count;
  this->directory = test.directory;
}

TestSuite::TestSuite(const TestSuite& other){
  this->tests = other.tests;
  this->name = other.name;
  this->exec_fail_count = other.exec_fail_count;
  this->directory = other.directory;
}
