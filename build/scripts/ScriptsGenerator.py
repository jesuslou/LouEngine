import os

VALID_PLATFORMS = ["win32", "osx"]
VALID_FRAMEWORKS = ["sfml", "sdl"]

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

cmake "{root_cmake_path}" -DCMAKE_CONFIGURATION_TYPES="{configuration}" {common_cmake_flags} {framework_flags} {platform_flags}

read -p "Press any key to continue..."
"""

update_file_template="""#!/bin/bash

if [ ! -d "{project_folder_path}" ]; then
	mkdir "{project_folder_path}"
fi

cd "{project_folder_path}"

cmake "{root_cmake_path}" -DCMAKE_CONFIGURATION_TYPES="{configuration}" {common_cmake_flags} {framework_flags} {platform_flags}

read -p "Press any key to continue..."
"""

common_cmake_flags = "-DENTITYX_BUILD_SHARED=0 -DENTITYX_BUILD_TESTING=0 -DBUILD_STATIC_LIBS=0 -DBUILD_SHARED_LIBS=1 -DJSONCPP_WITH_TESTS=0"

def generate_templated_file(script_file_path, template, configuration, script_folder_path, root_cmake_path, project_folder_path, common_cmake_flags, framework_flags, platform_flags):
	with open(script_file_path, "w", newline='') as file:
		file.write(template.format(
			configuration=configuration,
			script_folder_path=script_folder_path,
			root_cmake_path=root_cmake_path,
			project_folder_path=project_folder_path,
			common_cmake_flags=common_cmake_flags,
			framework_flags=framework_flags,
			platform_flags=platform_flags
		))

def create_platform_scripts(project_name, framework, deploy_path, platform, framework_static_lib):
	script_folder = path_to_os("{}/generate".format(deploy_path))

	platform_flags = ""
	if platform == "osx" or platform == "ios":
		platform_flags = "-G Xcode"

	script_folder_platform = path_to_os("{}/{}".format(script_folder, platform))
	generate_script_file_debug = path_to_os("{}/{}-debug-{}-generate.sh".format(script_folder_platform, platform, framework))
	generate_script_file_release = path_to_os("{}/{}-release-{}-generate.sh".format(script_folder_platform, platform, framework))
	update_script_file_debug = path_to_os("{}/{}-debug-{}-update.sh".format(script_folder_platform, platform, framework))
	update_script_file_release = path_to_os("{}/{}-release-{}-update.sh".format(script_folder_platform, platform, framework))
	project_folder_path_debug = path_to_os("{}/projects/{}-{}-{}-debug".format(deploy_path, project_name, platform, framework))
	project_folder_path_release = path_to_os("{}/projects/{}-{}-{}-release".format(deploy_path, project_name, platform, framework))

	create_folder_if_not_exists(script_folder_platform)

	framework_flags = ""
	if (framework == "sfml"):
		framework_flags = "-DBUILD_SHARED_LIBS={} -DUSE_SFML=1".format("0" if framework_static_lib else "1")
	elif (framework == "sdl"):
		framework_flags = "-DUSE_SDL=1 -DSDL_STATIC={} -DSDL_SHARED={} -DSDL_JOYSTICK=0 -DSDL_HAPTIC=0".format(
			"1" if framework_static_lib else "0", "0" if framework_static_lib else "1")

	root_cmake_path = path_to_os("{}/{}".format(deploy_path, project_name.lower()))

	generate_templated_file(generate_script_file_debug, generate_file_template, "Debug", script_folder_platform, root_cmake_path, project_folder_path_debug, common_cmake_flags, framework_flags, platform_flags)
	generate_templated_file(generate_script_file_release, generate_file_template, "Release", script_folder_platform, root_cmake_path, project_folder_path_release, common_cmake_flags, framework_flags, platform_flags)

	generate_templated_file(update_script_file_debug, update_file_template, "Debug", script_folder_platform, root_cmake_path, project_folder_path_debug, common_cmake_flags, framework_flags, platform_flags)
	generate_templated_file(update_script_file_release, update_file_template, "Release", script_folder_platform, root_cmake_path, project_folder_path_release, common_cmake_flags, framework_flags, platform_flags)

def create_framework_generate_scripts(project_name, deploy_path, framework_static_lib, frameworks, platforms):
	for platform in platforms:
		if platform in VALID_PLATFORMS:
			for framework in frameworks:
				framework = framework.lower();
				if framework in VALID_FRAMEWORKS:
					create_platform_scripts(project_name, framework, deploy_path, platform, framework_static_lib)
				else:
					print("Skipping scripts generation for '{}' [{}]. Invalid framework! Valid frameworks are {}".format(framework, platform, VALID_FRAMEWORKS))
		else:
			print("Skipping scripts generation for '{}'. Invalid platform! Valid platforms are {}".format(platform, VALID_PLATFORMS))
