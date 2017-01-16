cmake_minimum_required(VERSION 3.6)

function(log msg)
	set(verbose TRUE)
	if("${verbose}")
		MESSAGE( STATUS "${msg}")
	endif()
endfunction(log)

function(add_source_groups files)
	if(files)
		#log("files:          ${files}" )
		foreach(file ${files})
			add_file_to_source_group(${file})
		endforeach()
	endif()
endfunction(add_source_groups)

function(add_file_to_source_group file)
	#log("file:          ${file}" )
    get_filename_component (file_path ${file} PATH)
	#log("file_path:          ${file_path}" )
    file (RELATIVE_PATH rel_file_path ${CMAKE_CURRENT_LIST_DIR} ${file_path})
	#log("rel_file_path:          ${rel_file_path}" )
	if(NOT "${rel_file_path}" STREQUAL "")
		string (REPLACE "/" "\\" rel_file_path ${rel_file_path})
	endif()
	source_group ("${rel_file_path}" FILES ${file})
endfunction(add_file_to_source_group)

function (generate_static_library)
	cmake_parse_arguments(lib "" "name;dependencies_folder" "dependencies" ${ARGN})
	
	file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
	file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

	add_source_groups("${HEADERS}")
	add_source_groups("${SOURCES}")

	add_library("${lib_name}" STATIC ${HEADERS} ${SOURCES})
	target_include_directories ("${lib_name}" PUBLIC "./include")
	set_target_properties("${lib_name}" PROPERTIES LINKER_LANGUAGE CXX)
	
	add_target_dependencies("${lib_name}" "${lib_dependencies}" "${lib_dependencies_folder}")
	
endfunction(generate_static_library)

function (generate_game)
	cmake_parse_arguments(game "" "name;dependencies_folder" "dependencies" ${ARGN})

	project("${game_name}")

	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

	file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
	file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
	list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
	add_source_groups("${HEADERS}")
	add_source_groups("${SOURCES}")

	add_executable ("${game_name}" "${HEADERS}" "${SOURCES}")
	set_target_properties("${game_name}" PROPERTIES LINKER_LANGUAGE CXX)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${game_name}")
	add_definitions(-DSFML_STATIC)
	
	add_target_dependencies("${game_name}" "${game_dependencies}" "${game_dependencies_folder}")
	
endfunction(generate_game)

function(add_target_dependencies target_name dependencies dependencies_folder)
	if(dependencies)
		log("Dependencies for target ${target_name}: ${dependencies} -> ${dependencies_folder}/SFML/include")
		foreach(dependency ${dependencies})
			target_link_libraries("${target_name}" "${dependency}")
			check_add_dependency_include_folder("${dependency}" "sfml" "${dependencies_folder}/SFML/include")
			check_add_dependency_include_folder("${dependency}" "entityx" "${dependencies_folder}/entityx")
		endforeach()
	endif()
endfunction(add_target_dependencies)

function(check_add_dependency_include_folder dependency_name search_tag include_folder )
	string(FIND "${dependency_name}" "${search_tag}" pos)
	if(NOT "${pos}" STREQUAL "-1")
	log("++Including ${include_folder}")
		include_directories("${include_folder}")
	endif()
endfunction(check_add_dependency_include_folder)

function(add_dependency_subdirectory dependency_name output_dir)
	add_subdirectory("${output_dir}/${dependency_name}" "${CMAKE_CURRENT_BINARY_DIR}/${dependency_name}")
endfunction(add_dependency_subdirectory)
