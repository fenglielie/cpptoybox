add_executable(mlog_demo mlog_demo.cpp)
target_include_directories(mlog_demo PRIVATE ..)
zero_target_preset_definitions(mlog_demo)

add_test(NAME mlog_demo COMMAND mlog_demo)
