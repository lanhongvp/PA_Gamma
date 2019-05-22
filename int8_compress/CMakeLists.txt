cmake_minimum_required(VERSION 3.4.1)
project(headpose)

message($(PROJECT_SOURCE_DIR))
aux_source_directory(. DIR_SRCS)
add_subdirectory(src)
