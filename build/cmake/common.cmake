cmake_minimum_required(VERSION 3.6)

function(log msg)
	set(verbose TRUE)
	if("${verbose}")
		MESSAGE( STATUS "${msg}")
	endif()
endfunction(log)

function(add_source_groups files)
	if(files)
		foreach(file ${files})
			add_file_to_source_group(${file})
		endforeach()
	endif()
endfunction(add_source_groups)

function(add_file_to_source_group file)
    get_filename_component (file_path ${file} PATH)
    file (RELATIVE_PATH rel_file_path ${CMAKE_CURRENT_LIST_DIR} ${file_path})
	if(NOT "${rel_file_path}" STREQUAL "")
		string (REPLACE "/" "\\" rel_file_path ${rel_file_path})
	endif()
	source_group ("${rel_file_path}" FILES ${file})
endfunction(add_file_to_source_group)

function(add_target_dependencies target_name dependencies dependencies_folder)
	if(dependencies)
		foreach(dependency ${dependencies})
			target_link_libraries("${target_name}" "${dependency}")
		endforeach()
	endif()
endfunction(add_target_dependencies)

function(add_platform_files header_files source_files)
	if(WIN32)
		file(GLOB_RECURSE extra_headers "${CMAKE_CURRENT_SOURCE_DIR}/source/win32/*.h")
		file(GLOB_RECURSE extra_sources "${CMAKE_CURRENT_SOURCE_DIR}/source/win32/*.cpp")
	endif()
	if(extra_headers)
		set(header_files  "${header_files};${extra_headers}" CACHE INTERNAL "header_files")
	endif()
	if(extra_sources)
		set(source_files  "${source_files};${extra_sources}" CACHE INTERNAL "source_files")
	endif()
endfunction(add_platform_files)

function(add_framework_files header_files source_files)
	if(USE_SFML)
		file(GLOB_RECURSE extra_headers "${CMAKE_CURRENT_SOURCE_DIR}/frameworks/SFML/include/*.h")
		file(GLOB_RECURSE extra_sources "${CMAKE_CURRENT_SOURCE_DIR}/frameworks/SFML/source/*.cpp")
	elseif(USE_SDL)
		file(GLOB_RECURSE extra_headers "${CMAKE_CURRENT_SOURCE_DIR}/frameworks/SDL/include/*.h")
		file(GLOB_RECURSE extra_sources "${CMAKE_CURRENT_SOURCE_DIR}/frameworks/SDL/source/*.cpp")
	endif()
	if(extra_headers)
		set(header_files  "${header_files};${extra_headers}" CACHE INTERNAL "header_files")
	endif()
	if(extra_sources)
		set(source_files  "${source_files};${extra_sources}" CACHE INTERNAL "source_files")
	endif()
endfunction(add_framework_files)

function(add_framework_include_directories lib_name)
	if(USE_SFML)
		target_include_directories ("${lib_name}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/frameworks/SFML/include")
	elseif(USE_SDL)
		target_include_directories ("${lib_name}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/frameworks/SDL/include")
	endif()
endfunction(add_framework_include_directories)

function(add_msvc_precompiled_header precompiled_header precompiled_source project_sources)
	if(EXISTS ${precompiled_header} AND EXISTS ${precompiled_source})
		get_filename_component (precompiled_header_filename ${precompiled_header} NAME)
		log("Generating precompiler header ${precompiled_header_filename} for MSCV")
		foreach(src_file ${project_sources})
			set_source_files_properties(
				${src_file}
				PROPERTIES
				COMPILE_FLAGS "/Yu\"${precompiled_header_filename}\""
			)
		endforeach()

		set_source_files_properties (${precompiled_source}
				PROPERTIES HEADER_FILE_ONLY
				false
		)
		set_source_files_properties(
			${precompiled_source}
			PROPERTIES
			COMPILE_FLAGS "/Yc\"${precompiled_header_filename}\""
		)
	endif()
endfunction(add_msvc_precompiled_header)

function(add_precompiled_header lib_pch source_files)
	if (MSVC)
		add_msvc_precompiled_header("${CMAKE_CURRENT_SOURCE_DIR}/include/${lib_pch}.h" "${CMAKE_CURRENT_SOURCE_DIR}/source/win32/${lib_pch}.cpp" "${source_files}")
	endif()
endfunction()

function(warnings_as_errors TARGET)
    get_target_property(CURRENT_COMPILE_FLAGS ${TARGET} COMPILE_FLAGS)
    if("${CURRENT_COMPILE_FLAGS}" STREQUAL "CURRENT_COMPILE_FLAGS-NOTFOUND")
        set (CURRENT_COMPILE_FLAGS "")
    endif()

    #if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_HOST_APPLE OR EMSCRIPTEN) #CMAKE_HOST_APPLE for OSX (llvm) build..
    #    set_target_properties (${TARGET} PROPERTIES COMPILE_FLAGS "${CURRENT_COMPILE_FLAGS} -Werror")
    #    if ((NOT ANDROID) AND TARGET ${TARGET}_LIB)
    #        set_target_properties (${TARGET}_LIB PROPERTIES COMPILE_FLAGS "${CURRENT_COMPILE_FLAGS} -Werror")
    #    endif ((NOT ANDROID) AND TARGET ${TARGET}_LIB)
    #else
	if (MSVC)
        set_target_properties (${TARGET} PROPERTIES COMPILE_FLAGS "${CURRENT_COMPILE_FLAGS} /WX")
		if(TARGET ${TARGET}_LIB)
            set_target_properties (${TARGET}_LIB PROPERTIES COMPILE_FLAGS "${CURRENT_COMPILE_FLAGS} /WX")
        endif()
    endif()
endfunction(warnings_as_errors)
