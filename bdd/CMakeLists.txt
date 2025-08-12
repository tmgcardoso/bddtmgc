
cmake_minimum_required(VERSION 2.8)

project("BddTmgc")

set(EXECUTABLE_FILENAME "bddtmgc")

file(GLOB_RECURSE CPP_SOURCE "${CMAKE_SOURCE_DIR}/src/*.c??")

add_executable(
  ${EXECUTABLE_FILENAME}
  ${CPP_SOURCE}
)


