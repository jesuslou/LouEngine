cmake_minimum_required(VERSION 3.6)

include (${CMAKE_CURRENT_SOURCE_DIR}/build/common.cmake)

set(dependencies_folder "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/")

add_dependency_subdirectory("SFML" "${dependencies_folder}")
add_dependency_subdirectory("entityx" "${dependencies_folder}")

set(dependencies 
		"sfml-window"
		"sfml-system"
		"sfml-network"
		"sfml-graphics"
		"sfml-audio"
		"entityx"
	)
generate_static_library(name "LouEngine" dependencies "${dependencies}" dependencies_folder "${dependencies_folder}")
