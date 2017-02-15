cmake_minimum_required(VERSION 3.6)

set(project_name "LouEngine")

set(dependencies 
	# LouEngine
	"core"
	
	# Third Party
	"entityx"
	"jsoncpp_lib"
	"gmock"
	"gmock_main"
	"gtest"
	"gtest_main"
)
	
generate_static_library(name "${project_name}" dependencies "${dependencies}" dependencies_folder "${dependencies_folder}" pch "LouEnginePrecompile")
set_target_properties ("${project_name}" PROPERTIES FOLDER "${louengine_ide_group}")

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
