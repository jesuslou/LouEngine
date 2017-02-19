import argparse
import os

license = """////////////////////////////////////////////////////////////
//
// LouEngine - Multiplatform Game Engine Project
// Copyright (C) 2016-2017 Jesus Lou (loucocito@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

"""

def path_to_os(path):
	full_path = os.path.abspath(path)
	parts = full_path.split("/")
	return os.path.sep.join(parts)

def read_parameters():
	parser = argparse.ArgumentParser()
	parser.add_argument('-p', '--paths', required=True, nargs='+', help='Paths to look for files to add license')

	args = parser.parse_args()

	return args.paths

def add_license(filename, lines):
	print("Adding missing license to {}".format(filename))

	new_file_content = []
	new_file_content.append(license)
	new_file_content.extend(lines)

	with open(filename, "w") as file:
		for line in new_file_content:
			file.write(line)

def process_file(filename):
	lines = []
	with open(filename, "r") as file:
		lines = file.readlines()

	if '////////////////////////////////////////////////////////////' not in lines[0]:
		add_license(filename, lines)


def include_license_for_path(path):
	for root, dirs, files in os.walk(path, topdown=False):
		for name in files:
			filename = os.path.join(root, name)
			if ".h" in filename or ".cpp" in filename:
				process_file(filename)

def include_license(paths):
	for path in paths:
		path = path_to_os(path)
		if os.path.isdir(path):
			include_license_for_path(path)
		else:
			print("Invalid path! {} is not a valid folder".format(path))

if __name__ == '__main__':
	paths = read_parameters()
	include_license(paths)