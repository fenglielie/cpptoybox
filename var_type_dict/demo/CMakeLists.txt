add_executable(var_type_dict_demo var_type_dict.cpp)
target_include_directories(var_type_dict_demo PRIVATE ..)

add_test(NAME var_type_dict_demo COMMAND var_type_dict_demo)
