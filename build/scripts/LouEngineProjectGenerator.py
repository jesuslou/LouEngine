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
    parser.add_argument('-d', '--deploy_path', required=True, help='Path where the project will be created')
    parser.add_argument('-r', '--remote', default="none", help='Remote to initialize the project in')
    parser.add_argument('-p', '--push', action='store_true', help='whether the changes will be pushed to the specified remote')

    args = parser.parse_args()

    if args.remote == "none" and args.push == True:
        print("No remote specified. -p/--push option ignored.")
        args.push = False

    return args.name, args.deploy_path, args.remote, args.push

def create_folder_if_not_exists(path):
    if not os.path.exists(path):
        os.makedirs(path)

def create_project(project_name, deploy_path, remote, push):
    remove_folder_recursively(deploy_path)
    create_folder_if_not_exists(path_to_os(deploy_path))

    create_folder_if_not_exists(path_to_os("{}/dependencies".format(deploy_path)))
    create_folder_if_not_exists(path_to_os("{}/{}".format(deploy_path, project_name)))
    create_folder_if_not_exists(path_to_os("{}/{}/include".format(deploy_path, project_name)))
    create_folder_if_not_exists(path_to_os("{}/{}/source/common".format(deploy_path, project_name)))

    #if subprocess.call('git init', cwd=r"{}".format(deploy_path)) != 0:
    #    message_and_die('Cannot create a git repository in {}'.format(deploy_path))

    #if remote != "none":
    #    if subprocess.call('git remote add origin {}'.format(remote), cwd=r"{}".format(deploy_path)) != 0:
    #        message_and_die('Cannot set {} as remote for git repository in {}'.format(remote, deploy_path))

    #if subprocess.call('git submodule add https://github.com/jesuslou/LouEngine.git dependencies/LouEngine', cwd=r"{}".format(deploy_path)) != 0:
    #    message_and_die('Cannot add submodule LouEngine!')

    #if subprocess.call('git submodule update --init --recursive --force', cwd=r"{}".format(deploy_path)) != 0:
    #    message_and_die('Cannot update submodules!')

    with open(path_to_os("{}/.gitignore".format(deploy_path)), "w") as gitignore_file:
        gitignore_file.write("projects")

    with open(path_to_os("{}/{}/main.cpp".format(deploy_path, project_name)), "w") as gitignore_file:
        gitignore_file.write("#include <stdio.h>\n\n")
        gitignore_file.write("int main(int argc, char** argv)\n")
        gitignore_file.write("{\n")
        gitignore_file.write("\t printf(\"hello world\");\n")
        gitignore_file.write("}\n")

    with open(path_to_os("{}/{}/CMakeLists.txt".format(deploy_path, project_name)), "w") as cmake_file:
        cmake_file.write('cmake_minimum_required(VERSION 3.6)\n\n')
        cmake_file.write('set(dependencies_folder "${CMAKE_SOURCE_DIR}/../dependencies/")\n')
        cmake_file.write('include ("${dependencies_folder}/LouEngine/build/common.cmake")\n\n')
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
        generate_win_file.write('cmake ../../{} -DCMAKE_CONFIGURATION_TYPES="Debug;Release"\n'.format(project_name))
        generate_win_file.write('cd ../..\n')
        generate_win_file.write('pause\n')

    if push:
        if subprocess.call('git add .', cwd=r"{}".format(deploy_path)) != 0:
            message_and_die('Cannot add autogenerated content!')
        if subprocess.call('git commit -m "First commit"', cwd=r"{}".format(deploy_path)) != 0:
            message_and_die('Cannot commit!')
        if subprocess.call('git push -u origin master', cwd=r"{}".format(deploy_path)) != 0:
            message_and_die('Cannot push to {}!'.format(remote))

if __name__ == '__main__':
    check_git_in_path()
    check_cmake_in_path()
    name, deploy_path, remote, push = read_parameters()
    create_project(name, deploy_path, remote, push)