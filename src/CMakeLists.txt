#https://cmake.org/cmake/help/v2.8.12/cmake.html#section_Variables
#https://mirkokiefer.com/cmake-by-example-f95eb47d45b1

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
#SET(CMAKE_C_COMPILER "clang")
#SET(CMAKE_CXX_COMPILER "clang++")
PROJECT(SimFault CXX)
SET(CMAKE_BUILD_TYPE Release)

# Put everything in same folder
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/SimFault-bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SimFault-bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SimFault-bin)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SimFault-bin)

#Folders that have files to build
ADD_SUBDIRECTORY(simfault)
ADD_SUBDIRECTORY(lib)

