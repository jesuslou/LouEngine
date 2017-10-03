import ScriptsGenerator
import argparse
from os import sep

def generate_all(folder):
    project_name = folder.split(sep)[-1]
    generation_platforms = ["win32", "osx"]

    ScriptsGenerator.create_generate_scripts(project_name, folder, generation_platforms)
    ScriptsGenerator.create_generator_script(folder)
    ScriptsGenerator.create_rp_script(folder)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--folder', required=True, help="Project's name")
    args = parser.parse_args()
    generate_all(args.folder)
