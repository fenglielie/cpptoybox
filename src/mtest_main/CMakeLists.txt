add_library(mtest_main STATIC mtest_main.cpp)
target_include_directories(mtest_main PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
    $<INSTALL_INTERFACE:include>
)
