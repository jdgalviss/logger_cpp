cmake_minimum_required(VERSION 3.5)

project(Logger VERSION 0.1 DESCRIPTION "Logger Library for Connected Robotics test")
find_package (Threads)

include(GNUInstallDirs)

include_directories(
  include
  ./include
  ./src
)

# add the Test
add_executable(Tests tests/test1.cc
  src/logger.cc)

target_link_libraries (Tests ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(Tests PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_BINARY_DIR}/include"
                           )

add_library(Logger SHARED 
    src/logger.cc)

set_target_properties(Logger PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER include/logger.h)

target_link_libraries (Logger ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(Logger PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_BINARY_DIR}/include"
                           "${PROJECT_BINARY_DIR}/src"
                           )

install(TARGETS Logger
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
