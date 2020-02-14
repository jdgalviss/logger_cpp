cmake_minimum_required(VERSION 3.5)

# set the project name
project(Logger)
add_compile_options(-std=c++11)

include_directories(
  include
  ./include
  ./src
)

# add the executable
add_executable(Logger src/application.cc
  src/logger.cc
  src/configuration.cc)

target_include_directories(Logger PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_BINARY_DIR}/include"
                           )