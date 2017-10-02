#!/bin/bash
current_dir=$(pwd)
project_dir="${current_dir}/project-debug"

if [ ! -d "${project_dir}" ]; then
	mkdir "${project_dir}"
fi

cd "${project_dir}"

OSX_FLAG=""
PLATFORM=$(uname)
if [ "${PLATFORM}" == "Darwin" ]; then
	OSX_FLAG="-G"
fi

cmake "${current_dir}" -DCMAKE_CONFIGURATION_TYPES="Debug" -DENABLE_TESTS=0 -DCOPY_BINARY=0 ${OSX_FLAG}

read -p "Press any key to continue..."
