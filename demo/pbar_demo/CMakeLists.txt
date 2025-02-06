add_executable(pbar_demo pbar_demo.c)
target_link_libraries(pbar_demo PRIVATE pbar)

add_test(NAME pbar_demo COMMAND pbar_demo)
