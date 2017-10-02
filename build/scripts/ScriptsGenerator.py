import os

VALID_PLATFORMS = ["win32", "osx"]

def path_to_os(path):
	full_path = os.path.abspath(path)
	parts = full_path.split("/")
	return os.path.sep.join(parts)

def create_folder_if_not_exists(path):
	if not os.path.exists(path):
		os.makedirs(path)

generate_file_template="""#!/bin/bash

if [ -d "{project_folder_path}" ]; then
	set -e
	rm -rf "{project_folder_path}"
fi

mkdir "{project_folder_path}"

cd "{project_folder_path}"

cmake "{root_cmake_path}" -DCMAKE_CONFIGURATION_TYPES="{configuration}" {common_cmake_flags} {framework_flags} {platform_flags} -DENABLE_TESTS={enable_tests}

read -p "Press any key to continue..."
"""

update_file_template="""#!/bin/bash

if [ ! -d "{project_folder_path}" ]; then
	mkdir "{project_folder_path}"
fi

cd "{project_folder_path}"

cmake "{root_cmake_path}" -DCMAKE_CONFIGURATION_TYPES="{configuration}" {common_cmake_flags} {framework_flags} {platform_flags} -DENABLE_TESTS={enable_tests}

read -p "Press any key to continue..."
"""

common_cmake_flags = "-DENTITYX_BUILD_SHARED=0 -DENTITYX_BUILD_TESTING=0 -DBUILD_STATIC_LIBS=0 -DBUILD_SHARED_LIBS=1 -DJSONCPP_WITH_TESTS=0"

def generate_templated_file(script_file_path, template, configuration, script_folder_path, root_cmake_path, project_folder_path, common_cmake_flags, extra_flags, platform_flags):
	with open(script_file_path, "w", newline='') as file:
		file.write(template.format(
			configuration=configuration,
			script_folder_path=script_folder_path,
			root_cmake_path=root_cmake_path,
			project_folder_path=project_folder_path,
			common_cmake_flags=common_cmake_flags,
			framework_flags=extra_flags,
			platform_flags=platform_flags,
			enable_tests="1" if configuration == "Debug" else "0"
		))

def create_platform_scripts(project_name, deploy_path, platform):
	script_folder = path_to_os("{}/generate".format(deploy_path))

	platform_flags = ""
	if platform == "osx" or platform == "ios":
		platform_flags = "-G Xcode"

	script_folder_platform = path_to_os("{}/{}".format(script_folder, platform))
	generate_script_file_debug = path_to_os("{}/debug-01-generate.sh".format(script_folder_platform))
	generate_script_file_release = path_to_os("{}/release-01-generate.sh".format(script_folder_platform))
	update_script_file_debug = path_to_os("{}/debug-02-update.sh".format(script_folder_platform))
	update_script_file_release = path_to_os("{}/release-02-update.sh".format(script_folder_platform))
	project_folder_path_debug = path_to_os("{}/projects/{}-debug".format(deploy_path, platform))
	project_folder_path_release = path_to_os("{}/projects/{}-release".format(deploy_path, platform))

	create_folder_if_not_exists(script_folder_platform)

	extra_flags = ""

	root_cmake_path = path_to_os("{}/{}".format(deploy_path, project_name.lower()))

	generate_templated_file(generate_script_file_debug, generate_file_template, "Debug", script_folder_platform, root_cmake_path, project_folder_path_debug, common_cmake_flags, extra_flags, platform_flags)
	generate_templated_file(generate_script_file_release, generate_file_template, "Release", script_folder_platform, root_cmake_path, project_folder_path_release, common_cmake_flags, extra_flags, platform_flags)

	generate_templated_file(update_script_file_debug, update_file_template, "Debug", script_folder_platform, root_cmake_path, project_folder_path_debug, common_cmake_flags, extra_flags, platform_flags)
	generate_templated_file(update_script_file_release, update_file_template, "Release", script_folder_platform, root_cmake_path, project_folder_path_release, common_cmake_flags, extra_flags, platform_flags)

def create_generate_scripts(project_name, deploy_path, platforms):
	for platform in platforms:
		if platform in VALID_PLATFORMS:
			create_platform_scripts(project_name, deploy_path, platform)
		else:
			print("Skipping scripts generation for '{}'. Invalid platform! Valid platforms are {}".format(platform, VALID_PLATFORMS))
