cmake_minimum_required(VERSION 3.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/LouEngine.cmake)

set(dependencies_folder "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/")
set(packages_folder "${CMAKE_CURRENT_SOURCE_DIR}/packages/")

add_dependency_subdirectory("SFML" "${dependencies_folder}")

set(gtest_ide_group "GoogleTest")
add_dependency_subdirectory("googletest" "${dependencies_folder}")
set_target_properties ("gmock" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gmock_main" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gtest" PROPERTIES FOLDER "${gtest_ide_group}")
set_target_properties ("gtest_main" PROPERTIES FOLDER "${gtest_ide_group}")

# Third Party packages
add_dependency_subdirectory("jsoncpp" "${packages_folder}/thirdparty/")

# LouEngine packages
set(ide_group "LouEngine")
add_dependency_subdirectory("core" "${packages_folder}")
add_dependency_subdirectory("packer" "${packages_folder}")
add_dependency_subdirectory("engine" "${packages_folder}")
add_dependency_subdirectory("ECS" "${packages_folder}")

