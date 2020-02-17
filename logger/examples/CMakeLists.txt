cmake_minimum_required(VERSION 3.5)

# set the project name
project(Samples)
find_package (Threads)
add_compile_options(-std=c++11)

include_directories(
  include
  .
)

LINK_DIRECTORIES(usr/local/lib)

# add the Sample1
add_executable(Sample1 sample1.cc)

target_link_libraries (Sample1 ${CMAKE_THREAD_LIBS_INIT}
                                libLogger.so)


target_include_directories(Sample1 PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_BINARY_DIR}/include"
                           )

# add the Sample2
add_executable(logging_threads logging_threads.cc)

target_link_libraries (logging_threads ${CMAKE_THREAD_LIBS_INIT}
                                libLogger.so)


target_include_directories(logging_threads PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_BINARY_DIR}/include"
                           )