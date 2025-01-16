add_executable(mparser_demo mparser_demo.cpp)
target_include_directories(mparser_demo PRIVATE ..)

add_test(NAME mparser_demo COMMAND mparser_demo)
