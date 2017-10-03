import os


def path_to_os(path):
    full_path = os.path.abspath(path)
    parts = full_path.split("/")
    return os.path.sep.join(parts)


main_cpp_template="""#include <application/{application_class_name}.h>
#include <application/SApplicationWindowParameters.h>

{application_class_name} application; // This class is your starting point

int main(int argc, char** argv)
{{
    SApplicationWindowParameters applicationWindowParameters(800, 600, "{project_name}");
    application.Init(applicationWindowParameters);
    application.Update();
    application.Destroy();
}}
"""


application_h_template="""#pragma once

#include <application/CApplication.h>

class {application_class_name} : public CApplication
{{
public:
    {application_class_name}();

protected:
    bool InitProject(CGameSystems& gameSystems) override;
    void UpdateProject(float dt) override;
    void DestroyProject() override;
}};
"""


application_cpp_template="""#include <application/{application_class_name}.h>

{application_class_name}::{application_class_name}()
{{
}}

bool {application_class_name}::InitProject(CGameSystems& gameSystems)
{{
    (void)gameSystems;
    return true;
}}

void {application_class_name}::UpdateProject(float dt)
{{
    (void)dt;
}}

void {application_class_name}::DestroyProject()
{{
}}
"""


cmake_template="""cmake_minimum_required(VERSION 3.6)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${{CMAKE_BINARY_DIR}})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${{CMAKE_BINARY_DIR}})

set(dependencies_folder "${{CMAKE_SOURCE_DIR}}/../dependencies/")
include(${{dependencies_folder}}/LouEngine/build/cmake/LouEngine.cmake)

add_dependency_subdirectory("LouEngine" "${{dependencies_folder}}")

set(dependencies
    "LouEngine"
)

generate_game(name "{project_name}" dependencies "${{dependencies}}" dependencies_folder "${{dependencies_folder}}")
"""


packs_definition_template="""{
  "resourcesRootPath": "../../res",
  "outputDestinationPath": "../../res_output",
  "packerToolPath": "../../dependencies/LouEngine/tools",
  "version": 0,
  "packs": [],
  "dataPackFileTypes": [
    { "extension": "txt", "rule": [] },
    { "extension": "json", "rule": [] },
    { "extension": "xml", "rule": [] }
  ],
  "resourcePackFileTypes": [
    { "extension": "png", "rule": [] },
    { "extension": "jpeg", "rule": [] },
    { "extension": "wav", "rule": [] },
    { "extension": "ogg", "rule": [] }
  ]
}
"""


def generate_templated_files(project_name, root_path):
    with open(path_to_os("{}/.gitignore".format(root_path)), "w") as gitignore_file:
        gitignore_file.write("projects")

    application_class_name = "C{}Application".format(project_name)
    game_path = path_to_os("{}/{}".format(root_path, project_name.lower()))

    with open(path_to_os("{}/main.cpp".format(game_path)), "w") as main_cpp_file:
        main_cpp_file.write(main_cpp_template.format(application_class_name=application_class_name, project_name=project_name))

    with open(path_to_os("{}/include/application/{}.h".format(game_path, application_class_name)), "w") as app_h_file:
        app_h_file.write(application_h_template.format(application_class_name=application_class_name))

    with open(path_to_os("{}/source/common/application/{}.cpp".format(game_path, application_class_name)), "w") as app_cpp_file:
        app_cpp_file.write(application_cpp_template.format(application_class_name=application_class_name))

    with open(path_to_os("{}/CMakeLists.txt".format(game_path)), "w") as cmake_file:
        cmake_file.write(cmake_template.format(project_name=project_name))


def generate_packs_definition_file(root_path):
    with open(path_to_os("{}/res/packerTool/packsDefinition.json".format(root_path)), "w") as packs_definition_file:
        packs_definition_file.write(packs_definition_template)
