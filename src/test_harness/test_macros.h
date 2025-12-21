#ifndef _TEST_MACROS_H_
#define _TEST_MACROS_H_

#include <cstring>
#include <cmath>
#include <ostream>
#include <glm/vec3.hpp>
#include <iostream>

class TestHarness;

/*  *** Test Usage ***
 *  General Steps:
 *    1. In global scope, make tests using MAKE_TEST()
 *    2. In main, set the tests to run with TEST()
 *    3. Run the tests with RUN_ALL_TESTS();
 *
 *  MAKE_TEST Usage:
 *    MAKE_TEST(TEST_NAME)
 *      - TEST_NAME does NOT go into quotes.
 *      
 *      Within the curly braces you can use the check macros to define asserts
 *      You can write to the test logs using LOG << **details**;
 *      You can write to error with ERR << **details**; <- I do not recommend 
 *                                                         writing to ERR
 *
 *      I doubt it'll happen but never write to variables __kit__.__local_result__
 *      or __kit__.__test_result__ or even __kit__
 *
 *      In order to use LOG and ERR you must put TEST_START(); at the start of the test
 *  
 *  TEST Usage:
 *
 *    TEST(TEST_SUITE_NAME, TEST_NAME)
 *      - TEST_SUITE_NAME - The name of the test suite, can be repeated
 *                          DO NOT USE QUOTES
 *      - TEST_NAME - The name of the test, do not repeat.
 *                    DO NOT USE QUOTES
 *
 *  RUN_ALL_TESTS Usage:
 *      - put RUN_ALL_TESTS() in main.
 *  
 *  conditonal checks to use within MAKE_TEST code block
 *    CHECK_EQ
 *    CHECK_NEQ
 *    CHECK_LT
 *    CHECK_LTEQ
 *    CHECK_GT
 *    CHECK_GTEQ
 *    CHECK_TRUE
 *    CHECK_FALSE
 *    CHECK_AND
 *    CHECK_OR
 *    CHECK_LT_EPSILON
 *    CHECK_FLOAT_EQUAL
 *
*/


// ostream operator for glm::vec3
inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec) {
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}


#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define RUN_ALL_TESTS() TestHarness::run_all_tests();
#define TEST(test_suite_name, test_function) TestHarness::create_test(#test_suite_name, \
              #test_function, test_function)

#define CHECK_EQ(a, b) __kit__.__local_result__ = (a == b); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                      << a << " == " << b << "\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << a << " == " << b << "\n";

#define CHECK_NEQ(a, b) __kit__.__local_result__ = (a != b); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << a << " != " << b << "\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << a << " != " << b << "\n";

#define CHECK_LT(a, b) __kit__.__local_result__ = (a < b); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << a << " < " << b << "\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << a << " < " << b << "\n";

#define CHECK_LTEQ(a, b) __kit__.__local_result__ = (a <= b); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << a << " <= " << b << "\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << a << " <= " << b << "\n";

#define CHECK_GT(a, b) __kit__.__local_result__ = (a > b); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << a << " > " << b << "\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << a << " > " << b << "\n";

#define CHECK_GTEQ(a, b) __kit__.__local_result__ = (a >= b); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << a << " >= " << b << "\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << a << " >= " << b << "\n";

#define CHECK_AND(a, b) __kit__.__local_result__ = ((a) && (b)); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << (a) << " && " << (b) << "\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << (a) << " && " << (b) << "\n";

#define CHECK_OR(a, b) __kit__.__local_result__ = ((a) || (b)); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << (a) << " || " << (b) << "\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << (a) << " || " << (b) << "\n";

#define CHECK_TRUE(a) __kit__.__local_result__ = ((bool)(a)); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << (a) << " == true\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << (a) << " == true" << "\n";

#define CHECK_FALSE(a) __kit__.__local_result__ = (!(bool)(a)); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << (a) << " == false\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << (a) << " == false" << "\n";

#define __EPSILON__ 0.00001
#define CHECK_LT_EPSILON(a) __kit__.__local_result__ = ((a) < __EPSILON__); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                          << (a) << " < EPSILON\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << (a) << " < EPSILON" << "\n";

#define CHECK_FLOAT_EQUAL(a, b) __kit__.__local_result__ = (std::abs((a) - (b)) < __EPSILON__ && !std::isnan(a) && !std::isnan(b)); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                        "abs((" << (a) << ")-(" << (b) << ")) < EPSILON\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      "abs((" << (a) << ")-(" << (b) << ")) < EPSILON\n"; \

#define CHECK_APPROX_EQ_VEC3(a, b) __kit__.__local_result__ = ((std::abs((a).x - (b).x) < __EPSILON__ && !std::isnan((a).x) && !std::isnan((b).x)) && \
                                                              (std::abs((a).y - (b).y) < __EPSILON__ && !std::isnan((a).x) && !std::isnan((b).x)) && \
                                                              (std::abs((a).z - (b).z) < __EPSILON__ && !std::isnan((a).x) && !std::isnan((b).x))); \
                      __kit__.__test_result__ = __kit__.__test_result__ && __kit__.__local_result__; \
                      __kit__.LOG << __FILENAME__ << " " << __LINE__ << ": " << "test " \
                      << (a) << " == " << (b) << "\n"; \
                      if(!__kit__.__local_result__) __kit__.ERR << __FILENAME__ << " " << __LINE__ << ": " \
                      << (a) << " == " << (b) << "\n";

#define MAKE_TEST(test_name) void test_name(TestKit& __kit__)
#define TEST_START() std::ostream& LOG  = __kit__.LOG; std::ostream& ERR = __kit__.ERR;
#define ABORT_IF_FAILING() if(!__kit__.__test_result__) {__kit__.ERR << "Early abort due to failure " \
                      << __LINE__ << "\n"; return;}

#endif
