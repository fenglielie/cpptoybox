include(CheckCXXSourceCompiles)

set(TEST_FORMAT_CODE
    "#include <format>\n"
    "int main() {\n"
    "    std::format(\"Hello, {}!\", \"world\");\n"
    "    return 0;\n"
    "}\n"
)

check_cxx_source_compiles("${TEST_FORMAT_CODE}" FORMAT_SUPPORTED)

if(FORMAT_SUPPORTED)
    add_executable(progressbar_demo progressbar_demo.cpp)
    target_include_directories(progressbar_demo PRIVATE ..)

    add_test(NAME progressbar_demo COMMAND progressbar_demo)
else()
    message(WARNING "The compiler does not support C++20 <format>, skipping target 'progressbar_demo'.")
endif()
