add_executable(mtest_demo mtest_demo.cpp)
target_include_directories(mtest_demo PRIVATE ..)
target_link_libraries(mtest_demo PRIVATE mtest_main)
