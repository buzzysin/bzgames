bz_test(test_core_logger
  PARENT
    tests
  DEPENDS
    bz::core
    GTest::gtest_main
  SOURCES
    src/test.cpp
    src/test.h
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GoogleTest)
gtest_discover_tests(bz_test_core_logger)