cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../build/cmake/LouEngine.cmake)

set(sdl_dependencies_folder "${CMAKE_CURRENT_SOURCE_DIR}")
set(sdl_ide_group "SDL")

set(sdl_lib "SDL2")
if(SDL_STATIC)
	set(sdl_lib "SDL2-static")
endif()

# SDL2
add_dependency_subdirectory("SDL2" "${sdl_dependencies_folder}")
set_target_properties ("SDL2main" PROPERTIES FOLDER "${sdl_ide_group}")
set_target_properties ("${sdl_lib}" PROPERTIES FOLDER "${sdl_ide_group}")
#set_target_properties ("uninstall" PROPERTIES FOLDER "${sdl_ide_group}")

# GLM
add_dependency_subdirectory("glm" "${sdl_dependencies_folder}")
set_target_properties ("glm_dummy" PROPERTIES FOLDER "${sdl_ide_group}")	

# SDL_mixer
set(SDL2_LIBRARY "${sdl_lib}")
set(SDL2_INCLUDE_DIR "${sdl_dependencies_folder}/SDL2/include")
add_dependency_subdirectory("SDL_mixer" "${sdl_dependencies_folder}")
set(sdl_mixer_ide_group "SDL/SDL_mixer")

function (set_output_directory_and_ide_group lib_name ide_group)
	set_target_properties ("${lib_name}" PROPERTIES FOLDER "${ide_group}")
	set_target_properties ("${lib_name}"
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	)
endfunction(set_output_directory_and_ide_group)

set_output_directory_and_ide_group("FLAC" "${sdl_mixer_ide_group}")
set_output_directory_and_ide_group("LIBMAD" "${sdl_mixer_ide_group}")
set_output_directory_and_ide_group("MIKMOD" "${sdl_mixer_ide_group}")
set_output_directory_and_ide_group("MODPLUG" "${sdl_mixer_ide_group}")
set_output_directory_and_ide_group("OGG" "${sdl_mixer_ide_group}")
set_output_directory_and_ide_group("SDLMIXER" "${sdl_mixer_ide_group}")
set_output_directory_and_ide_group("VORBIS" "${sdl_mixer_ide_group}")
set_output_directory_and_ide_group("VORBIS_FILE" "${sdl_mixer_ide_group}")
set_output_directory_and_ide_group("SMPEG" "${sdl_mixer_ide_group}")


#SDL_image
set(lib_name "SDL_image")

file(GLOB_RECURSE header_files "${CMAKE_CURRENT_SOURCE_DIR}/SDL_image/*.h")
file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/SDL_image/*.c")

list(REMOVE_ITEM source_files "${CMAKE_CURRENT_SOURCE_DIR}/SDL_image/showimage.c")

add_source_groups("${header_files}")
add_source_groups("${source_files}")

add_library("${lib_name}" SHARED ${header_files} ${source_files})
target_include_directories ("${lib_name}" PUBLIC "SDL_image")
	
set(dependencies 
	"${sdl_lib}"
	)
link_sdl_libraries("${lib_name}" "${dependencies}" "${sdl_dependencies_folder}")

set_target_properties("${lib_name}" PROPERTIES LINKER_LANGUAGE C)
set_target_properties ("SDL_image" PROPERTIES FOLDER "${sdl_ide_group}")
