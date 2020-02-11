cmake_minimum_required(VERSION 3.10)

# set the project name
project(Logger VERSION 0.1 DESCRIPTION "Logging library")

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