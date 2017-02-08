cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../build/cmake/LouEngine.cmake)

set(sdl_dependencies_folder "${CMAKE_CURRENT_SOURCE_DIR}")
set(sdl_ide_group "SDL")

# SDL2
add_dependency_subdirectory("SDL2" "${sdl_dependencies_folder}")
set_target_properties ("SDL2main" PROPERTIES FOLDER "${sdl_ide_group}")
set_target_properties ("SDL2-static" PROPERTIES FOLDER "${sdl_ide_group}")

# GLM
add_dependency_subdirectory("glm" "${sdl_dependencies_folder}")
set_target_properties ("glm_static" PROPERTIES FOLDER "${sdl_ide_group}")	


#SDL_image
set(lib_name "SDL_image")

file(GLOB_RECURSE header_files "${CMAKE_CURRENT_SOURCE_DIR}/SDL_image/*.h")
file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/SDL_image/*.c")

list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/SDL_image/showimage.c")

add_source_groups("${header_files}")
add_source_groups("${source_files}")

add_library("${lib_name}" STATIC ${header_files} ${source_files})
target_include_directories ("${lib_name}" PUBLIC "SDL_image")
	
set(dependencies 
	"SDL2-static"
	)
link_sdl_libraries("${lib_name}" "${dependencies}" "${sdl_dependencies_folder}")

set_target_properties("${lib_name}" PROPERTIES LINKER_LANGUAGE C)
set_target_properties ("SDL_image" PROPERTIES FOLDER "${sdl_ide_group}")
