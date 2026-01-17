include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)


add_executable(
  unit_tests
  iterator_test.cpp
)
target_link_libraries(
  unit_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
