import sys
import os
from stat import S_IWUSR
import argparse
import subprocess
from subprocess import DEVNULL
import importlib

app_folder, script_name = os.path.split(os.path.abspath(sys.argv[0]))

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
	parser.add_argument('-rf', '--remove_folder', action='store_true', help='Remove folder if already exists')
	parser.add_argument('-of', '--override_files', action='store_true', help="Override autogenerated files for tempalte project (main.cpp, app.h/cpp, CMakeLists.txt)")
	parser.add_argument('-me', '--max_entities', required=False, help="Max number of entities. Maximum allowed is 8192. Default is 4096", default="4096")
	parser.add_argument('-mt', '--max_tags', required=False, help="Max number of registrable tags. Rounded to highest power of two. Default is 64", default="64")

	args = parser.parse_args()

	return args.name, args.deploy_path, args.remote, args.remove_folder, args.override_files, args.max_entities, args.max_tags

def create_folder_if_not_exists(path):
	if not os.path.exists(path):
		os.makedirs(path)

def create_project(project_name, deploy_path, remote, remove_folder, override_files, max_entities, max_tags ):
	deploy_path = "{}/{}".format(deploy_path, project_name)

	max_entities = 8192 if int(max_entities) > 8192 else int(max_entities)

	max_tags_rounded = 1
	while (max_tags_rounded < int(max_tags)):
		max_tags_rounded *= 2

	if remove_folder:
		print("Removing folder under {}".format(deploy_path))
		remove_folder_recursively(deploy_path)

	create_folder_if_not_exists(path_to_os(deploy_path))
	create_folder_if_not_exists(path_to_os("{}/dependencies".format(deploy_path)))

	game_path = "{}/{}".format(deploy_path, project_name.lower())
	create_folder_if_not_exists(path_to_os(game_path))

	repo_already_checked_out = os.listdir(path_to_os(game_path)) != []

	create_folder_if_not_exists(path_to_os("{}/include/application".format(game_path)))
	create_folder_if_not_exists(path_to_os("{}/source/common/application".format(game_path)))

	if repo_already_checked_out == False and subprocess.call('git init', shell=True, cwd=r"{}".format(deploy_path)) != 0:
		message_and_die('Cannot create a git repository in {}'.format(deploy_path))

	if remote != "none":
		if repo_already_checked_out:
			if subprocess.call('git remote set-url origin {}'.format(remote), shell=True, cwd=r"{}".format(deploy_path)) != 0:
				message_and_die('Cannot change repository remote to {} in {}'.format(remote, deploy_path))
		elif subprocess.call('git remote add origin {}'.format(remote), shell=True, cwd=r"{}".format(deploy_path)) != 0:
				message_and_die('Cannot set {} as remote for git repository in {}'.format(remote, deploy_path))

	if repo_already_checked_out == False and subprocess.call('git submodule add https://github.com/jesuslou/LouEngine.git dependencies/LouEngine', shell=True, cwd=r"{}".format(deploy_path)) != 0:
		message_and_die('Cannot add submodule LouEngine!')

	if repo_already_checked_out == False and subprocess.call('git submodule update --init --recursive --force', shell=True, cwd=r"{}".format(deploy_path)) != 0:
		message_and_die('Cannot update submodules!')

	if override_files:
		TemplateProjectFilesGenerator = importlib.import_module("{}.dependencies.LouEngine.build.scripts.TemplateProjectFilesGenerator".format(project_name))
		TemplateProjectFilesGenerator.generate_templated_files(project_name, deploy_path)
		TemplateProjectFilesGenerator.generate_packs_definition_file(deploy_path)

	ScriptsGenerator = importlib.import_module("{}.dependencies.LouEngine.build.scripts.ScriptsGenerator".format(project_name))
	generation_platforms = ["win32", "osx"]
	ScriptsGenerator.create_generate_scripts(project_name, deploy_path, generation_platforms, max_entities, max_tags_rounded)

	deploy_path_absolute = path_to_os(app_folder + "/" + project_name + "/")
	ScriptsGenerator.create_generator_script(deploy_path_absolute, max_entities, max_tags_rounded)
	ScriptsGenerator.create_rp_script(deploy_path_absolute)


if __name__ == '__main__':
	check_git_in_path()
	check_cmake_in_path()
	name, deploy_path, remote, remove_folder, override_files, max_entities, max_tags = read_parameters()
	create_project(name, deploy_path, remote, remove_folder, override_files, max_entities, max_tags)
