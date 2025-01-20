cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(allay VERSION 0.1)
include(cmake/zero.cmake)

zero_init_quiet()
zero_info()

zero_add_subdirs_rec(src)

include(CTest)
zero_add_subdirs_rec(demos)
