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
    target_link_libraries(progressbar_demo PRIVATE progressbar)

    add_test(NAME progressbar_demo COMMAND progressbar_demo)
else()
    message(STATUS "The compiler does not support C++20 <format>, skipping target 'progressbar_demo'.")
endif()
