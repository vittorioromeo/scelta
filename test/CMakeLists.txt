# Copyright (c) 2017 Vittorio Romeo
# MIT License |  https://opensource.org/licenses/MIT
# http://vittorioromeo.info | vittorio.romeo@outlook.com

# Add a custom target for the unit tests.
add_custom_target(tests COMMENT "Build all the unit tests.")

# `check` depends on `tests`.
add_dependencies(check tests)

# Include directories.
include_directories(${SCELTA_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE INSTALL_FILES_LIST "${CMAKE_CURRENT_LIST_DIR}/scelta/testbed/*.hpp")
file(GLOB_RECURSE INSTALL_FILES_LIST "${CMAKE_CURRENT_LIST_DIR}/scelta/*.hpp")
add_library(HEADER_ONLY_TARGET3 STATIC ${INSTALL_FILES_LIST})
set_source_files_properties(${INSTALL_FILES_LIST} PROPERTIES HEADER_FILE_ONLY 1)
set_target_properties(HEADER_ONLY_TARGET3 PROPERTIES LINKER_LANGUAGE CXX)

# Generate all the unit tests.
vrm_cmake_generate_unit_tests_glob("${SCELTA_SOURCE_DIR}/test/*.cpp")

macro(augment_test x xname flags)
#{
    set(AUGMENTED_TEST_NAME "${x}_${xname}")
    get_target_property(xsrcs ${x} SOURCES)

    add_executable("${AUGMENTED_TEST_NAME}" EXCLUDE_FROM_ALL ${xsrcs})
    vrm_cmake_add_unit_test("${AUGMENTED_TEST_NAME}" ${CMAKE_CURRENT_BINARY_DIR}/${AUGMENTED_TEST_NAME})
    target_compile_options("${AUGMENTED_TEST_NAME}" PUBLIC ${ARGN})
#}
endmacro()

# Link `pthread` to every generated test.
foreach(_x IN LISTS vrm_cmake_out)
#{
    target_compile_options(${_x} PUBLIC "-fdiagnostics-color=always")

    # TODO: to utility functions in vrm_cmake? to option?
    if(DEFINED AUGMENT_TESTS AND NOT DEFINED WIN_MINGW_DEVEL)
    #{
        vrm_cmake_augment_test_asan(${_x})
        vrm_cmake_augment_test_ofast(${vrm_cmake_out})

        vrm_cmake_augment_test_tsan(${_x})
        vrm_cmake_augment_test_ofast(${vrm_cmake_out})

        vrm_cmake_augment_test_ubsan(${_x})
        vrm_cmake_augment_test_ofast(${vrm_cmake_out})
    #}
    endif()
#}
endforeach()
