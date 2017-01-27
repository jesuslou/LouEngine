cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_LIST_DIR}/common.cmake)

function (generate_static_library)
	cmake_parse_arguments(lib "" "name;dependencies_folder;pch" "dependencies" ${ARGN})
	
	file(GLOB_RECURSE header_files "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
	file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/source/common/*.cpp")
	set(header_files  "${header_files}" CACHE INTERNAL "header_files")
	set(source_files  "${source_files}" CACHE INTERNAL "source_files")
	add_platform_files("${header_files}" "${source_files}")
	
	add_source_groups("${header_files}")
	add_source_groups("${source_files}")

	if(lib_pch)
		add_precompiled_header("${lib_pch}" "${source_files}")
	endif()
	
	add_library("${lib_name}" STATIC ${header_files} ${source_files})
	target_include_directories ("${lib_name}" PUBLIC "include")
	set_target_properties("${lib_name}" PROPERTIES LINKER_LANGUAGE CXX)
	
	add_target_dependencies("${lib_name}" "${lib_dependencies}" "${lib_dependencies_folder}")
	
endfunction(generate_static_library)

function (generate_game)
	cmake_parse_arguments(game "" "name;dependencies_folder;pch" "dependencies" ${ARGN})

	# .lib with all game source, to use as well in unit tests
	set(lib_name "${game_name}_LIB")
	generate_static_library(name "${lib_name}" dependencies "${game_dependencies}" dependencies_folder "${game_dependencies_folder}" pch "${game_pch}")
	
	# game itself
	project("${game_name}")

	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

	set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
	add_source_groups("${SOURCES}")

	add_executable ("${game_name}" "${SOURCES}")
	set_target_properties("${game_name}" PROPERTIES LINKER_LANGUAGE CXX)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${game_name}")
	add_definitions(-DSFML_STATIC)
	
	add_target_dependencies("${game_name}" "${lib_name}" "${game_dependencies_folder}")
	
endfunction(generate_game)

function(add_dependency_subdirectory dependency_name dependency_dir)
	add_subdirectory("${dependency_dir}/${dependency_name}" "${CMAKE_BINARY_DIR}/${dependency_name}")
endfunction(add_dependency_subdirectory)
