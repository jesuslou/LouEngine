cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/LouEngine.cmake)

set(dependencies_folder "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/")

add_definitions(-DBUILD_SHARED_LIBS=0)
add_dependency_subdirectory("SFML" "${dependencies_folder}")

add_definitions(-DENTITYX_BUILD_SHARED=0)
add_definitions(-DENTITYX_BUILD_TESTING=0)
add_dependency_subdirectory("entityx" "${dependencies_folder}")

set(gtest_ide_group "GoogleTest")
add_dependency_subdirectory("googletest" "${dependencies_folder}")
set_target_properties ("gmock" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gmock_main" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gtest" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gtest_main" PROPERTIES FOLDER "${gtest_ide_group}")

set(dependencies 
	# Third Party
	"sfml-window"
	"sfml-system"
	"sfml-network"
	"sfml-graphics"
	"sfml-audio"
	"entityx"
	"gmock"
	"gmock_main"
	"gtest"
	"gtest_main"
	)
generate_static_library(name "LouEngine" dependencies "${dependencies}" dependencies_folder "${dependencies_folder}" pch "LouEnginePrecompile")
