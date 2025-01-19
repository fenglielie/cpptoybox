add_executable(progressbar_demo progressbar_demo.cpp)
target_include_directories(progressbar_demo PRIVATE ..)

add_test(NAME progressbar_demo COMMAND progressbar_demo)
