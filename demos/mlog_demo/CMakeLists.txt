add_executable(mlog_demo mlog_demo.cpp)
target_link_libraries(mlog_demo PRIVATE mlog)
zero_target_preset_definitions(mlog_demo)

add_test(NAME mlog_demo COMMAND mlog_demo)
