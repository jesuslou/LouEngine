import ScriptsGenerator
import argparse
import math
from os import sep

def generate_all(folder, max_entities, max_tags):
    project_name = folder.split(sep)[-1]
    generation_platforms = ["win32", "osx"]

    max_entities = 8192 if int(max_entities) > 8192 else int(max_entities)

    max_tags_rounded = 1
    while (max_tags_rounded < int(max_tags)):
        max_tags_rounded *= 2

    print("- max_entities: {}".format(max_entities))
    print("- max_tags: {}".format(max_tags_rounded))

    ScriptsGenerator.create_generate_scripts(project_name, folder, generation_platforms, max_entities, max_tags_rounded)
    ScriptsGenerator.create_generator_script(folder, max_entities, max_tags_rounded)
    ScriptsGenerator.create_rp_script(folder)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--folder', required=True, help="Project's name")
    parser.add_argument('--max_entities', required=False, help="Max number of entities. Maximum allowed is 8192. Default is 4096", default="4096")
    parser.add_argument('--max_tags', required=False, help="Max number of registrable tags. Rounded to highest power of two. Default is 64", default="64")
    args = parser.parse_args()
    generate_all(args.folder, args.max_entities, args.max_tags)
