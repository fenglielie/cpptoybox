add_executable(simple_thread_pool_demo simple_thread_pool_demo.cpp)
target_include_directories(simple_thread_pool_demo PRIVATE ..)

find_package(Threads REQUIRED)
target_link_libraries(simple_thread_pool_demo PRIVATE Threads::Threads)

add_test(NAME simple_thread_pool_demo COMMAND simple_thread_pool_demo)
