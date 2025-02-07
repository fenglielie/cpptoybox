add_executable(gaussquad_test gaussquad_test.cpp)
target_link_libraries(gaussquad_test PRIVATE gaussquad)

add_test(NAME gaussquad_test COMMAND gaussquad_test)
