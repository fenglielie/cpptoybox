cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(allay VERSION 0.1)
include(cmake/zero.cmake)

zero_init_quiet()
zero_info()

zero_add_subdirs_rec(src)

include(CTest)
zero_add_subdirs_rec(demos)

set(ALLAY_TARGETS
    mtest
    safe_input
    msignal
    mlog
    data_handler
    var_type_dict
    simple_thread_pool
    mtracer
    mparser
    ini_parser
    progressbar
)

install(TARGETS ${ALLAY_TARGETS} EXPORT allayTargets)
install(EXPORT allayTargets
        FILE allayConfig.cmake
        NAMESPACE allay::
        DESTINATION lib/cmake/allay)
install(DIRECTORY include/ DESTINATION include/allay)
