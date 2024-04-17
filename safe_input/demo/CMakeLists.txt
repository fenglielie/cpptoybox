add_executable(safe_input_demo safe_input_demo.cpp)
target_include_directories(safe_input_demo PRIVATE ..)

add_test(NAME safe_input_demo COMMAND safe_input_demo)
