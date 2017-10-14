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

if(MAX_ENTITIES)
	add_definitions(-DMAX_ENTITIES=${MAX_ENTITIES})
else()
	add_definitions(-DMAX_ENTITIES=4096)
endif()

if(MAX_TAGS)
	add_definitions(-DMAX_TAGS=${MAX_TAGS})
else()
	add_definitions(-DMAX_TAGS=64)
endif()

warnings_as_errors("${project_name}")
