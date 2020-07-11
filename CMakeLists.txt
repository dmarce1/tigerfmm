# Copyright (c) 2015 Thomas Heller
# Copyright (c) 2015 Dominic Marcello
# Copyright (c) 2018-2019 Parsa Amini
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.12.4)
cmake_policy(VERSION 3.12.4)

# Libraries linked via full path no longer produce linker search paths.
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0074 NEW)

project(tigerfmm CXX)
enable_language(CUDA)

# Search path for CMake modules to be loaded by include() and find_package()
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

find_package(HPX REQUIRED NO_CMAKE_PACKAGE_REGISTRY)
find_package(Silo REQUIRED)
find_package(Boost REQUIRED)


set(tigerfmm_source_files
	src/main.cpp
	src/tree.cpp
)

set(tigerfmm_header_files
	tigerfmm/defs.hpp  
	tigerfmm/expansion.hpp  
	tigerfmm/multipole.hpp	
	tigerfmm/particle.hpp	
	tigerfmm/real.hpp  
	tigerfmm/simd.hpp
	tigerfmm/tree.hpp
)



add_hpx_executable(
  tigerfmm
  DEPENDENCIES
    Silo::silo Boost::boost
  SOURCES
    ${tigerfmm_source_files}
  HEADERS
    ${tigerfmm_header_files}
 )



target_include_directories(tigerfmm PUBLIC ${PROJECT_SOURCE_DIR})
target_compile_options(tigerfmm PUBLIC "-march=native")

