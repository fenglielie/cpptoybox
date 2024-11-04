cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(cpptoybox VERSION 0.1)
include(cmake/zero.cmake)

zero_init_quiet()
zero_info()

include(CTest)

add_subdirectory(mtest)
add_subdirectory(safe_input)
add_subdirectory(msignal)
add_subdirectory(mlog)
add_subdirectory(data_handler)
add_subdirectory(var_type_dict)
add_subdirectory(simple_thread_pool)
