# zero.cmake
# fenglielie@qq.com
# 2024-11-04

## marcos

macro(zero_usage)
    message(STATUS "\n"
        "    ###################################\n"
        "    #                                 #\n"
        "    #          CMakeZero 1.1          #\n"
        "    #                                 #\n"
        "    ###################################\n")

    message(STATUS "macro usage:\n"
        "   - zero_usage(): print this usage\n"
        "   - zero_init(): print usage, then init the project (call after project)\n"
        "   - zero_init_quiet(): init the project (call after project)\n"
        "   - zero_info(): show infomation\n"
        "   - zero_use_bin_subdir(): use bin/debug as runtime output directory when debug\n")

    message(STATUS "function usage:\n"
        "   - zero_add_subdirs(src): go to src/CMakeLists.txt and src/*/CMakeLists.txt\n"
        "   - zero_add_subdirs_rec(src): go to src/CMakeLists.txt and src/*/*/CMakeLists.txt (recurse)\n"
        "   - zero_get_files(tmp test): search source files in test/ |-> tmp\n"
        "   - zero_get_files_rec(tmp test): search source files in test/ and test/*/ |-> tmp (recurse)\n")

    message(STATUS "target function usage:\n"
        "   - zero_target_preset_definitions(targetname): add some definitions\n"
        "     * ZERO_TARGET_NAME=targetname\n"
        "     * ZERO_PROJECT_SOURCE_DIR=PROJECT_SOURCE_DIR\n"
        "     * ZERO_CURRENT_SOURCE_DIR=CMAKE_CURRENT_SOURCE_DIR\n"
        "   - zero_target_use_postfix(targetname): add postfix _d when Debug (default for library)\n"
        "   - zero_target_reset_output(targetname RUNTIME path): change RUNTIME(|ARCHIVE|LIBRARY) output to path\n"
        "   - zero_target_info(targetname): show target properties\n")

endmacro()

macro(zero_init)
    zero_usage()
    zero_init_quiet()
endmacro()

macro(zero_init_quiet)
    message(STATUS ">> Init Project: ${PROJECT_NAME} ${PROJECT_VERSION}")

    if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
        message(FATAL_ERROR "The binary directory cannot be the same as source directory")
    endif()

    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS ">> default CMAKE_BUILD_TYPE: \"Release\"")
        set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
    endif()

    # keep use folders in build/
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # create compile_commands.json in build/
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # c++ standard = C++20(required)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # libfunc
    # libfunc (release)
    set(CMAKE_DEBUG_POSTFIX "_d") # libfunc_d (debug)

    # ./bin
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # ./lib
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

    # ./lib
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

    # c/c++ compile flags
    if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"))
        set(common_flags  "-Wall -Wextra -Wfatal-errors -Wshadow -Wno-unused-parameter")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(common_flags "/W3 /WX /MP /utf-8")
    else()
        set(common_flags "")
    endif()

    # empty
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${common_flags}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${common_flags}")

    # debug default: -g
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${common_flags}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${common_flags}")

    # release default: -O3 -DNDEBUG
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${common_flags}")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${common_flags}")

endmacro()

macro(zero_info)
    message(STATUS "---------- <Check Information Begin> ----------")
    message(STATUS ">> system = ${CMAKE_SYSTEM_NAME}")
    message(STATUS ">> generator = ${CMAKE_GENERATOR}")
    message(STATUS ">> build_type = ${CMAKE_BUILD_TYPE}")
    message(STATUS ">> cxx_compiler_id = ${CMAKE_CXX_COMPILER_ID}(${CMAKE_CXX_COMPILER_VERSION})")
    message(STATUS ">> cxx_compiler = ${CMAKE_CXX_COMPILER}")
    message(STATUS ">> cxx_flags = " ${CMAKE_CXX_FLAGS})
    message(STATUS ">> cxx_flags_debug = " ${CMAKE_CXX_FLAGS_DEBUG})
    message(STATUS ">> cxx_flags_release = " ${CMAKE_CXX_FLAGS_RELEASE})
    message(STATUS ">> linker = ${CMAKE_LINKER}")
    message(STATUS ">> exe_linker_flags = ${CMAKE_EXE_LINKER_FLAGS}")
    message(STATUS "-----------------------------------------------")
endmacro()

macro(zero_use_bin_subdir)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin")
endmacro()

## functions

function(zero_get_files rst _sources)
    set(tmp_rst "")

    foreach(item ${_sources})
        if(IS_DIRECTORY ${item}) # item is dir
            file(GLOB itemSrcs CONFIGURE_DEPENDS
                ${item}/*.c ${item}/*.C ${item}/*.cc ${item}/*.cpp ${item}/*.cxx
                ${item}/*.h ${item}/*.hpp
                ${item}/*.f90
            )
            list(APPEND tmp_rst ${itemSrcs})
        else() # item is file
            # make sure using absolute filename
            if(NOT (IS_ABSOLUTE "${item}"))
                get_filename_component(item "${item}" ABSOLUTE)
            endif()

            list(APPEND tmp_rst ${item})
        endif()
    endforeach()

    set(${rst} ${tmp_rst} PARENT_SCOPE) # return
endfunction()

function(zero_get_files_rec rst _sources)
    set(tmp_rst "")

    foreach(item ${_sources})
        if(IS_DIRECTORY ${item}) # item is dir
            file(GLOB_RECURSE itemSrcs CONFIGURE_DEPENDS
                ${item}/*.c ${item}/*.C ${item}/*.cc ${item}/*.cpp ${item}/*.cxx
                ${item}/*.h ${item}/*.hpp
                ${item}/*.f90
            )
            list(APPEND tmp_rst ${itemSrcs})
        else() # item is file
            # make sure using absolute filename
            if(NOT (IS_ABSOLUTE "${item}"))
                get_filename_component(item "${item}" ABSOLUTE)
            endif()

            list(APPEND tmp_rst ${item})
        endif()
    endforeach()

    set(${rst} ${tmp_rst} PARENT_SCOPE) # return
endfunction()

# go to all relative subdirs which contains CMakeLists.txt from CMAKE_CURRENT_SOURCE_DIR.(not recurse)
# may not ordered as you want.
function(zero_add_subdirs _path)
    # search all subdirs
    file(GLOB children LIST_DIRECTORIES ON CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_path}/*)
    set(dirs "")
    list(PREPEND children "${CMAKE_CURRENT_SOURCE_DIR}/${_path}") # add first

    # append to dirs if contains CMakeLists.txt
    foreach(item ${children})
        if((IS_DIRECTORY ${item}) AND (EXISTS "${item}/CMakeLists.txt"))
            list(APPEND dirs ${item})
        endif()
    endforeach()

    # go to subdirs
    foreach(dir ${dirs})
        message(STATUS ">> Go to ${dir}")
        add_subdirectory(${dir})
    endforeach()

endfunction()

# go to all relative subdirs which contains CMakeLists.txt from CMAKE_CURRENT_SOURCE_DIR.(recurse)
# may not ordered as you want.
function(zero_add_subdirs_rec _path)
    # search all subdirs
    file(GLOB_RECURSE children LIST_DIRECTORIES ON CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_path}/*)
    set(dirs "")
    list(PREPEND children "${CMAKE_CURRENT_SOURCE_DIR}/${_path}") # add first

    # append to dirs if contains CMakeLists.txt
    foreach(item ${children})
        if((IS_DIRECTORY ${item}) AND (EXISTS "${item}/CMakeLists.txt"))
            list(APPEND dirs ${item})
        endif()
    endforeach()

    # go to subdirs
    foreach(dir ${dirs})
        message(STATUS ">> Go to ${dir}")
        add_subdirectory(${dir})
    endforeach()

endfunction()

## target functions

function(zero_inside_check_target _target)
    if(NOT TARGET "${_target}")
        message(FATAL_ERROR "${_target} is not a target")
    endif()
endfunction()

function(zero_target_preset_definitions _target)
    zero_inside_check_target(${_target})

    target_compile_definitions(${_target} PRIVATE
        "ZERO_TARGET_NAME=\"${_target}\""
        "ZERO_PROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\""
        "ZERO_CURRENT_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\""
    )
endfunction()

function(zero_target_use_postfix _target)
    zero_inside_check_target(${_target})

    set_target_properties(${_target}
        PROPERTIES
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    )
endfunction()

function(zero_target_reset_output _target _type _path)
    zero_inside_check_target(${_target})

    string(TOUPPER "${_type}" _type)

    if(${_type} STREQUAL "RUNTIME")
        set_target_properties(${_target}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${_path}"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${_path}"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${_path}"
        )
    elseif(${_type} STREQUAL "ARCHIVE")
        set_target_properties(${_target}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${_path}"
            ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${_path}"
            ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${_path}"
        )
    elseif(${_type} STREQUAL "LIBRARY")
        set_target_properties(${_target}
            PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${_path}"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${_path}"
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${_path}"
    )
    else()
        message(FATAL_ERROR "illegal type ${_type} (RUNTIME|ARCHIVE|LIBRARY)")
    endif()

endfunction()

function(zero_inside_list_print)
    # TITLE
    # PREFIX a
    # STRS a;b;c

    set(options "")
    set(oneValueArgs TITLE PREFIX)
    set(multiValueArgs STRS)
    cmake_parse_arguments("ARG" "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # return if STRS is empty
    list(LENGTH ARG_STRS strsLength)
    if(NOT strsLength)
        return()
    endif()

    # print title
    if(NOT (${ARG_TITLE} STREQUAL ""))
        message(STATUS ${ARG_TITLE})
    endif()

    # print prefix+item in STRS
    foreach(str ${ARG_STRS})
        message(STATUS "${ARG_PREFIX}${str}")
    endforeach()

endfunction()

function(zero_inside_print_property _target _porperty)
    string(TOUPPER "${_porperty}" _porperty)

    get_target_property(tmp ${_target} ${_porperty})
    if(NOT (tmp STREQUAL "tmp-NOTFOUND"))
        string(TOLOWER "${_porperty}" porperty_lower)
        zero_inside_list_print(STRS "${tmp}" TITLE  "${porperty_lower}:" PREFIX "  * ")
    endif()

    get_target_property(tmp ${_target} INTERFACE_${_porperty})
    if(NOT (tmp STREQUAL "tmp-NOTFOUND"))
        string(TOLOWER "${_porperty}" porperty_lower)
        zero_inside_list_print(STRS "${tmp}" TITLE  "interface_${porperty_lower}:" PREFIX "  * ")
    endif()

endfunction()

function(zero_target_info _target)
    zero_inside_check_target(${_target})

    message(STATUS "---------- <Check Target Begin> ----------")
    message(STATUS "name: ${_target}")

    get_target_property(tmp ${_target} TYPE)
    string(TOLOWER "${tmp}" tmp)
    message(STATUS "type: ${tmp}")

    get_target_property(tmp ${_target} SOURCE_DIR)
    message(STATUS "location: ${tmp}")

    zero_inside_print_property(${_target} SOURCES)
    zero_inside_print_property(${_target} INCLUDE_DIRECTORIES)
    zero_inside_print_property(${_target} LINK_DIRECTORIES)
    zero_inside_print_property(${_target} LINK_LIBRARIES)
    zero_inside_print_property(${_target} LINK_OPTIONS)
    zero_inside_print_property(${_target} COMPILE_OPTIONS)
    zero_inside_print_property(${_target} COMPILE_DEFINITIONS)

    message(STATUS "------------------------------------------")
endfunction()
