cmake_minimum_required(VERSION 3.6)

set(project_name "graphics")

generate_static_library(name "graphics")
set_target_properties ("${project_name}" PROPERTIES FOLDER "${louengine_ide_group}")

if(USE_SFML)
	set(sfml_dependencies 
		"sfml-window"
		"sfml-graphics"
		)
	link_sfml_libraries("${project_name}" "${sfml_dependencies}" "${dependencies_folder}/SFML")
elseif(USE_SDL)
	set(sdl_dependencies 
	"SDL2"
	"glm"
	"SDL_image"
	)
	link_sdl_libraries("${project_name}" "${sdl_dependencies}" "${dependencies_folder}/SDL")
endif()
