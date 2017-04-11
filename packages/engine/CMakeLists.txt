cmake_minimum_required(VERSION 3.6)

set(project_name "LouEngine")

set(dependencies 
	# LouEngine
	"core"
	"packer"
	
	# Third Party
	"entityx"
	"jsoncpp"
	"gmock"
	"gmock_main"
	"gtest"
	"gtest_main"
)
	
generate_static_library(name "${project_name}" dependencies "${dependencies}" dependencies_folder "${dependencies_folder}" pch "LouEnginePrecompile")
set_target_properties ("${project_name}" PROPERTIES FOLDER "${ide_group}")


set(sfml_dependencies 
	"sfml-window"
	"sfml-system"
	"sfml-network"
	"sfml-graphics"
	"sfml-audio"
	)
link_sfml_libraries("${project_name}" "${sfml_dependencies}" "${dependencies_folder}/SFML")


warnings_as_errors("${project_name}")
