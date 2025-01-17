add_executable(ini_parser_demo ini_parser_demo.cpp)
target_include_directories(ini_parser_demo PRIVATE ..)
zero_target_preset_definitions(ini_parser_demo)

# add_test(NAME ini_parser_demo COMMAND ini_parser_demo)
