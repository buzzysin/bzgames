bz_test(test_engine_common_input
  PARENT
    tests
  DEPENDS
    bz::core
    bz::engine::common
    GTest::gtest_main
  SOURCES
    src/test.cpp
    src/test.h
)

include(GoogleTest)
gtest_discover_tests(bz_test_engine_common_input)