cmake_minimum_required(VERSION 3.6)

set(louengine_root_folder "${CMAKE_CURRENT_SOURCE_DIR}/../../../")

include(${louengine_root_folder}/build/cmake/LouEngine.cmake)

set(dependencies_folder "${louengine_root_folder}/dependencies/")
set(packages_folder "${louengine_root_folder}/packages/")

set(project_name "PackerTool")

# Third Party packages
add_dependency_subdirectory("jsoncpp" "${packages_folder}/thirdparty/")

# LouEngine packages
set(ide_group "LouEngine")
add_dependency_subdirectory("core" "${packages_folder}")
add_dependency_subdirectory("packer" "${packages_folder}")

set(dependencies
	"core"
	"packer"
)

generate_executable(name "${project_name}" dependencies "${dependencies}" dependencies_folder "${dependencies_folder}")

if (COPY_BINARY)
	add_custom_command (TARGET ${project_name}
		POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/Release/PackerTool.exe" "${louengine_root_folder}/tools/"
	)
endif()
