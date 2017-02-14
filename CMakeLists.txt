cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/LouEngine.cmake)

set(project_name "LouEngine")
set(dependencies_folder "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/")

set(USE_SFML OFF CACHE BOOL "Generate solution using SFML")
set(USE_SDL OFF CACHE BOOL "Generate solution using SDL")

if(USE_SFML)
	log("Using SFML libraries")
	add_dependency_subdirectory("SFML" "${dependencies_folder}")
elseif(USE_SDL)
	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		log("=============================================")
		log("SDL IS NOT SUPPORTED IN OSX! TRY SFML INSTEAD")
		log("=============================================")
		set_target_properties ("sdl-not-supported" PROPERTIES FOLDER "")
		return()
	else()
		log("Using SDL libraries")
		add_dependency_subdirectory("SDL" "${dependencies_folder}")
	endif()
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
elseif(USE_SDL)
	set(sdl_dependencies 
	"SDL2"
	"glm"
	"SDL_image"
	"SDLMIXER"
	)
	link_sdl_libraries("${project_name}" "${sdl_dependencies}" "${dependencies_folder}/SDL")
endif()
