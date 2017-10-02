cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_LIST_DIR}/common.cmake)

function (generate_static_library)
	cmake_parse_arguments(lib "" "name;dependencies_folder;pch" "dependencies" ${ARGN})
	
	log("Generating ${lib_name}")
	
	file(GLOB_RECURSE header_files "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
	file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/source/common/*.cpp")
	set(header_files  "${header_files}" CACHE INTERNAL "header_files")
	set(source_files  "${source_files}" CACHE INTERNAL "source_files")
	add_platform_files("${header_files}" "${source_files}")
	add_framework_files("${header_files}" "${source_files}")
	
	add_source_groups("${header_files}")
	add_source_groups("${source_files}")

	if(lib_pch)
		add_precompiled_header("${lib_pch}" "${source_files}")
	endif()
	
	add_library("${lib_name}" STATIC ${header_files} ${source_files})
	target_include_directories ("${lib_name}" PUBLIC "include")
	target_include_directories ("${lib_name}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/frameworks/SFML/include")
	
	set_target_properties("${lib_name}" PROPERTIES LINKER_LANGUAGE CXX)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	
	add_target_dependencies("${lib_name}" "${lib_dependencies}" "${lib_dependencies_folder}")
	
	generate_tests("${lib_name}")
	
endfunction(generate_static_library)

function (generate_executable)
	cmake_parse_arguments(executable "" "name;dependencies_folder;pch" "dependencies" ${ARGN})

	# .lib with all game source, to use as well in unit tests
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
		set(lib_name "${executable_name}_LIB")
		generate_static_library(name "${lib_name}" dependencies "${executable_dependencies}" dependencies_folder "${executable_dependencies_folder}" pch "${executable_pch}")
	endif()
	
	# game itself
	project("${executable_name}")

	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

	set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
	add_source_groups("${SOURCES}")

	add_executable ("${executable_name}" "${SOURCES}")
	set_target_properties("${executable_name}" PROPERTIES LINKER_LANGUAGE CXX)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${executable_name}")
	if(NOT BUILD_SHARED_LIBS)
		add_definitions(-DSFML_STATIC)
	endif()
	
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
		add_target_dependencies("${executable_name}" "${lib_name}" "${executable_dependencies_folder}")
	else()
		add_target_dependencies("${executable_name}" "${executable_dependencies}" "${executable_dependencies_folder}")
	endif()
	
	generate_tests("${executable_name}")
	
endfunction(generate_executable)

function(add_dependency_subdirectory dependency_name dependency_dir)
	add_subdirectory("${dependency_dir}/${dependency_name}" "${CMAKE_BINARY_DIR}/${dependency_name}")
endfunction(add_dependency_subdirectory)

function(link_sfml_libraries target_name sfml_libraries sfml_folder)
	foreach(dependency ${sfml_libraries})
		target_link_libraries("${target_name}" "${dependency}")
	endforeach()
	target_include_directories("${target_name}" PUBLIC "${sfml_folder}/include")
	add_definitions(-DSFML_STATIC)
endfunction(link_sfml_libraries)
