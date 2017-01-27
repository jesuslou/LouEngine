import sys
import os
from stat import S_IWUSR
import argparse
import subprocess
from subprocess import DEVNULL

def path_to_os(path):
	full_path = os.path.abspath(path)
	parts = full_path.split("/")
	return os.path.sep.join(parts)

def remove_folder_recursively(path_to_folder):
	path_to_folder_exists = os.path.isdir(path_to_folder)
	if path_to_folder_exists:
		print("removing " + path_to_folder)
		if os.path.isdir(path_to_folder):
			for root, dirs, files in os.walk(path_to_folder, topdown=False):
				for name in files:
					filename = os.path.join(root, name)
					os.chmod(filename, S_IWUSR)
					os.remove(filename)
				for name in dirs:
					os.rmdir(os.path.join(root, name))
			os.rmdir(path_to_folder)

def message_and_die(message):
    print('\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
    print(message)
    print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n')
    sys.exit(1)

def check_git_in_path():
    if subprocess.call('git --version', shell=True, stdout=DEVNULL, stderr=DEVNULL) != 0:
        message_and_die('Git executable "git" not in your path')

def check_cmake_in_path():
    if subprocess.call('cmake --version', shell=True, stdout=DEVNULL, stderr=DEVNULL) != 0:
        message_and_die('CMake executable "cmake" not in your path')

def read_parameters():
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--name', required=True, help="Project's name")
    parser.add_argument('-d', '--deploy_path', default=".", help='Path where the project will be created under folder with name -n/--name')
    parser.add_argument('-r', '--remote', default="none", help='Remote to initialize the project in')
    parser.add_argument('-p', '--push', action='store_true', help='whether the changes will be pushed to the specified remote')
    parser.add_argument('-ge', '--git_enabled', action='store_true', help='Git actions should be performed')
    parser.add_argument('-g', '--generate', action='store_true', help='Generate the solution when finished')
    parser.add_argument('-rf', '--remove_folder', action='store_true', help='Remove folder if already exists')

    args = parser.parse_args()

    if args.remote == "none" and args.push == True:
        print("No remote specified. -p/--push option ignored.")
        args.push = False

    return args.name, args.deploy_path, args.remote, args.push, args.git_enabled, args.generate, args.remove_folder

def create_folder_if_not_exists(path):
    if not os.path.exists(path):
        os.makedirs(path)

def create_project(project_name, deploy_path, remote, push, git_enabled, generate, remove_folder):
    deploy_path = "{}/{}".format(deploy_path, project_name)
    print(deploy_path)
    if remove_folder:
        remove_folder_recursively(deploy_path)

    create_folder_if_not_exists(path_to_os(deploy_path))
    create_folder_if_not_exists(path_to_os("{}/dependencies".format(deploy_path)))

    game_path = "{}/{}".format(deploy_path, project_name.lower())
    create_folder_if_not_exists(path_to_os(game_path))
    create_folder_if_not_exists(path_to_os("{}/include/application".format(game_path)))
    create_folder_if_not_exists(path_to_os("{}/source/common/application".format(game_path)))

    if git_enabled:
        if subprocess.call('git init', cwd=r"{}".format(deploy_path)) != 0:
            message_and_die('Cannot create a git repository in {}'.format(deploy_path))

        if remote != "none":
            if subprocess.call('git remote add origin {}'.format(remote), cwd=r"{}".format(deploy_path)) != 0:
                message_and_die('Cannot set {} as remote for git repository in {}'.format(remote, deploy_path))

        if subprocess.call('git submodule add https://github.com/jesuslou/LouEngine.git dependencies/LouEngine', cwd=r"{}".format(deploy_path)) != 0:
            message_and_die('Cannot add submodule LouEngine!')

        if subprocess.call('git submodule update --init --recursive --force', cwd=r"{}".format(deploy_path)) != 0:
            message_and_die('Cannot update submodules!')


    with open(path_to_os("{}/.gitignore".format(deploy_path)), "w") as gitignore_file:
        gitignore_file.write("projects")

    application_class_name = "C{}Application".format(project_name)

    with open(path_to_os("{}/{}/main.cpp".format(deploy_path, project_name)), "w") as main_cpp_file:
        main_cpp_file.write('#include <application\{}.h>\n\n'.format(application_class_name))
        main_cpp_file.write('{} application;\n\n'.format(application_class_name))
        main_cpp_file.write('int main(int argc, char** argv)\n')
        main_cpp_file.write('{\n')
        main_cpp_file.write('\tapplication.Init("{}", 640, 480);\n'.format(project_name))
        main_cpp_file.write('\tapplication.Update();\n')
        main_cpp_file.write('\tapplication.Destroy();\n')
        main_cpp_file.write('}\n')

    with open(path_to_os("{}/include/application/{}.h".format(game_path, application_class_name)), "w") as app_h_file:
        app_h_file.write('#pragma once\n\n')
        app_h_file.write('#include <core/application/CApplication.h>\n\n')
        app_h_file.write('class {} : public CApplication\n'.format(application_class_name))
        app_h_file.write('{\n')
        app_h_file.write('public:\n')
        app_h_file.write('\t{}();\n\n'.format(application_class_name))
        app_h_file.write('protected:\n')
        app_h_file.write('\tbool InitProject() override;\n')
        app_h_file.write('\tvoid UpdateProject() override;\n')
        app_h_file.write('\tvoid DestroyProject() override;\n')
        app_h_file.write('};\n')

    with open(path_to_os("{}/source/common/application/{}.cpp".format(game_path, application_class_name)), "w") as app_cpp_file:
        app_cpp_file.write('#include <application/{}.h>\n\n'.format(application_class_name))
        app_cpp_file.write('{}::{}()\n{{\n\t\n}}\n\n'.format(application_class_name, application_class_name))
        app_cpp_file.write('bool {}::InitProject()\n{{\n\treturn true;\n}}\n\n'.format(application_class_name))
        app_cpp_file.write('void {}::UpdateProject()\n{{\n\t\n}}\n\n'.format(application_class_name))
        app_cpp_file.write('void {}::DestroyProject()\n{{\n\t\n}}\n'.format(application_class_name))

    with open(path_to_os("{}/CMakeLists.txt".format(game_path)), "w") as cmake_file:
        cmake_file.write('cmake_minimum_required(VERSION 3.6)\n\n')
        cmake_file.write('set(dependencies_folder "${CMAKE_SOURCE_DIR}/../dependencies/")\n')
        cmake_file.write('include(${dependencies_folder}/LouEngine/build/cmake/LouEngine.cmake)\n\n')
        cmake_file.write('add_dependency_subdirectory("LouEngine" "${dependencies_folder}")\n\n')
        cmake_file.write('set(dependencies\n')
        cmake_file.write('\t"LouEngine"\n')
        cmake_file.write(')\n\n')
        cmake_file.write('generate_game(name "{}" dependencies "${{dependencies}}" dependencies_folder "${{dependencies_folder}}")\n'.format(project_name))

    with open(path_to_os("{}/generate_win.bat".format(deploy_path)), "w") as generate_win_file:
        folder_name = "{}-win32".format(project_name)
        generate_win_file.write('@echo off\n\n')
        generate_win_file.write('IF NOT EXIST "projects/{}" (\n'.format(folder_name))
        generate_win_file.write('\tmkdir "projects/{}"\n'.format(folder_name))
        generate_win_file.write(')\n\n')
        generate_win_file.write('cd projects/{}\n'.format(folder_name))
        generate_win_file.write('cmake ../../{} -DCMAKE_CONFIGURATION_TYPES="Debug;Release" -DBUILD_SHARED_LIBS=0 -DENTITYX_BUILD_SHARED=0 -DENTITYX_BUILD_TESTING=0\n'.format(project_name))
        generate_win_file.write('start {}.sln\n'.format(project_name))
        generate_win_file.write('cd ../..\n')
        generate_win_file.write('pause\n')

    if git_enabled and push:
        if subprocess.call('git add .', cwd=r"{}".format(deploy_path)) != 0:
            message_and_die('Cannot add autogenerated content!')
        if subprocess.call('git commit -m "First commit"', cwd=r"{}".format(deploy_path)) != 0:
            message_and_die('Cannot commit!')
        if subprocess.call('git push -u origin master', cwd=r"{}".format(deploy_path)) != 0:
            message_and_die('Cannot push to {}!'.format(remote))

    if generate and os.path.isdir(path_to_os("{}/dependencies/LouEngine/dependencies/SFML".format(deploy_path))):
        print("Deploy finished. Starting project generation...")
        os.system("cd {} && generate_win.bat".format(deploy_path))

if __name__ == '__main__':
    check_git_in_path()
    check_cmake_in_path()
    name, deploy_path, remote, push, git_enabled, generate, remove_folder = read_parameters()
    create_project(name, deploy_path, remote, push, git_enabled, generate, remove_folder)