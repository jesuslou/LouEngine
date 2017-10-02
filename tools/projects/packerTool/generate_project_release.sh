#!/bin/bash
current_dir=$(pwd)
project_dir="${current_dir}/project-release"

if [ ! -d "${project_dir}" ]; then
	mkdir "${project_dir}"
fi

cd "${project_dir}"

OSX_FLAG=""
PLATFORM=$(uname)
if [ "${PLATFORM}" == "Darwin" ]; then
	OSX_FLAG="-G"
fi

cmake "${current_dir}" -DCMAKE_CONFIGURATION_TYPES="Release" -DENABLE_TESTS=0 -DCOPY_BINARY=1 ${OSX_FLAG}

read -p "Press any key to continue..."
