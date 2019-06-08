cmake_minimum_required(VERSION 2.8)
project(LibSimFault CXX)
set(CMAKE_BUILD_TYPE Release)

message("******** Check SimFault libs dependencies ********")

#Flags
# Vectorial optimization for new Processors -mavx -mxop -mfma4
# Theese flags must be removed if compile in old procs
SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -march=native -pedantic -O2 -fopenmp -pipe -D_FORTIFY_SOURCE=2 -floop-interchange -floop-strip-mine -floop-block")# -mavx -mxop -mfma4")


#Check C++14 standard support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++14 support.")
endif()

#Boost
find_package(Boost REQUIRED)
if (Boost_NOTFOUND)
	message(FATAL_ERROR "Boost library not found")
endif()

#Eigen
find_package(Eigen3 REQUIRED)
if (Eigen3_NOTFOUND)
	message(FATAL_ERROR "Eigen3 library not found")
endif()

#Find OpenMP
FIND_PACKAGE(OpenMP)
IF (OPENMP_FOUND)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -floop-parallelize-all -ftree-parallelize-loops=4")
ENDIF()

file(GLOB SOURCES "*.cpp")
add_library(LibSimFault SHARED ${SOURCES})
SET_TARGET_PROPERTIES(LibSimFault PROPERTIES PREFIX "")



        