# Copyright (c) 2017 Vittorio Romeo
# License: Academic Free License ("AFL") v. 3.0
# AFL License page: http://opensource.org/licenses/AFL-3.0
# http://vittorioromeo.info | vittorio.romeo@outlook.com

cmake_minimum_required(VERSION 3.6)

# Include `vrm_cmake`.
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake/"
    "${CMAKE_SOURCE_DIR}/../vrm_cmake/cmake/"
    "${CMAKE_SOURCE_DIR}/extlibs/vrm_cmake/cmake/")

include(vrm_cmake)

# Basic project setup.
project(scelta CXX)
enable_testing()

vrm_cmake_init_project_name_upper()
vrm_cmake_init_project_source_dir()
vrm_cmake_init_project_common_module_paths()

# vrm_cmake_find_extlib(vrm_pp)
# vrm_cmake_find_extlib(vrm_core)

# Alias the include directory.
set(SCELTA_INC_DIR "${SCELTA_SOURCE_DIR}/include/")
include_directories("${SCELTA_INC_DIR}")

# Other compiler flags.
vrm_cmake_add_common_compiler_flags()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
vrm_cmake_add_common_compiler_flags_suggest_attribute()

# The `check` target runs all tests.
vrm_check_target()

# Setup subdirectories.
add_subdirectory(test)

# Create header-only install target (automatically glob)
vrm_cmake_header_only_install_glob("${SCELTA_INC_DIR}" "include")