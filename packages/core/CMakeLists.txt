cmake_minimum_required(VERSION 3.6)

generate_static_library(name "core")
set_target_properties ("core" PROPERTIES FOLDER "${ide_group}")
warnings_as_errors("core")
