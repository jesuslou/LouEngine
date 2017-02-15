cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/LouEngine.cmake)

set(dependencies_folder "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/")
set(packages_folder "${CMAKE_CURRENT_SOURCE_DIR}/packages/")

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

add_dependency_subdirectory("jsoncpp" "${dependencies_folder}")

set(gtest_ide_group "GoogleTest")
add_dependency_subdirectory("googletest" "${dependencies_folder}")
set_target_properties ("gmock" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gmock_main" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gtest" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gtest_main" PROPERTIES FOLDER "${gtest_ide_group}")

# LouEngine packages
set(louengine_ide_group "LouEngine")
add_dependency_subdirectory("core" "${packages_folder}")
add_dependency_subdirectory("packer" "${packages_folder}")
add_dependency_subdirectory("engine" "${packages_folder}")
