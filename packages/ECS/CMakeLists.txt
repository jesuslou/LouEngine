cmake_minimum_required(VERSION 3.6)

set(project_name "ECS")

set(dependencies 
	# LouEngine
	"core"
	
	# Third Party
	"jsoncpp"
)
	
generate_static_library(name "${project_name}" dependencies "${dependencies}" dependencies_folder "${dependencies_folder}" pch "LouEnginePrecompile")
set_target_properties ("${project_name}" PROPERTIES FOLDER "${ide_group}")

if(NOT DEFINED MAX_ENTITIES)
	set(MAX_ENTITIES 4096)
endif()
target_compile_definitions("${project_name}" PUBLIC -DMAX_ENTITIES=${MAX_ENTITIES})

if(NOT DEFINED MAX_TAGS)	
	set(MAX_TAGS 64)
endif()
target_compile_definitions("${project_name}" PUBLIC -DMAX_TAGS=${MAX_TAGS})
		
warnings_as_errors("${project_name}")
