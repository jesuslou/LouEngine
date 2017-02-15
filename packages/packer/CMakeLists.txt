cmake_minimum_required(VERSION 3.6)

set(project_name "packer")

set(dependencies 
	# LouEngine
	"core"
	
	# Third Party
	"jsoncpp_lib"
)
	
generate_static_library(name "${project_name}" dependencies "${dependencies}" dependencies_folder "${dependencies_folder}")
set_target_properties ("${project_name}" PROPERTIES FOLDER "${ide_group}")

warnings_as_errors("${project_name}")
