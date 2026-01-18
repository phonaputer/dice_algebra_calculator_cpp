include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.17.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/src)

add_executable(
  unit_tests
  iterator_test.cpp
  lexer_test.cpp
  ${CMAKE_SOURCE_DIR}/src/lexer.cpp
  parser_test.cpp
  ${CMAKE_SOURCE_DIR}/src/parser.cpp
)
target_link_libraries(
  unit_tests
  GTest::gtest_main
  GTest::gmock
)

include(GoogleTest)
gtest_discover_tests(unit_tests)
