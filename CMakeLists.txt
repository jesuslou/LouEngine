cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/LouEngine.cmake)

set(project_name "LouEngine")
set(dependencies_folder "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/")

set(USE_SFML OFF CACHE BOOL "Generate solution using SFML")
set(USE_SDL2 OFF CACHE BOOL "Generate solution using SDL2")

if(USE_SFML)
	log("Using SFML libraries")
	add_dependency_subdirectory("SFML" "${dependencies_folder}")
elseif(USE_SDL2)
	log("Using SDL2 libraries")
	set(sdl_dependencies_folder "${dependencies_folder}/SDL/")
	set(sdl2_ide_group "SDL")
	add_dependency_subdirectory("SDL2" "${sdl_dependencies_folder}")
	#set_target_properties ("SDL2" PROPERTIES FOLDER "${sdl2_ide_group}") #only exists with SDL_SHARED=1
	set_target_properties ("SDL2main" PROPERTIES FOLDER "${sdl2_ide_group}")
	set_target_properties ("SDL2-static" PROPERTIES FOLDER "${sdl2_ide_group}")
	add_dependency_subdirectory("glm" "${sdl_dependencies_folder}")
	set_target_properties ("glm_static" PROPERTIES FOLDER "${sdl2_ide_group}")
	add_dependency_subdirectory("SDL_image" "${sdl_dependencies_folder}")
endif()

add_dependency_subdirectory("entityx" "${dependencies_folder}")

set(gtest_ide_group "GoogleTest")
add_dependency_subdirectory("googletest" "${dependencies_folder}")
set_target_properties ("gmock" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gmock_main" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gtest" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gtest_main" PROPERTIES FOLDER "${gtest_ide_group}")

set(dependencies 
	# Third Party
	"entityx"
	"gmock"
	"gmock_main"
	"gtest"
	"gtest_main"
	)

generate_static_library(name "${project_name}" dependencies "${dependencies}" dependencies_folder "${dependencies_folder}" pch "LouEnginePrecompile")
warnings_as_errors("${project_name}")

if(USE_SFML)
set(sfml_dependencies 
	"sfml-window"
	"sfml-system"
	"sfml-network"
	"sfml-graphics"
	"sfml-audio"
	)
	link_sfml_libraries("${project_name}" "${sfml_dependencies}" "${dependencies_folder}/SFML")
elseif(USE_SDL2)
	set(sdl_dependencies 
	"SDL2-static"
	"glm_static"
	)
	link_sdl2_libraries("${project_name}" "${sdl_dependencies}" "${dependencies_folder}/SDL")
endif()
