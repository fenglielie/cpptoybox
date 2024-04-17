add_executable(msignal_demo msignal_demo.cpp)
target_include_directories(msignal_demo PRIVATE ..)

add_test(NAME msignal_demo COMMAND msignal_demo)
