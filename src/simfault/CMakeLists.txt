CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(SimFault CXX)
SET(CMAKE_BUILD_TYPE Release)

MESSAGE("******** Check SimFault main dependencies ********")

#Flags
SET(CMAKE_CXX_FLAGS_RELEASE "-Wall -march=native -mtune=native -std=c++17 -lpthread -llapacke -llapack -lopenblas -lblas -lgfortran -no-pie -pedantic -O2 -pipe -fopenmp -D_FORTIFY_SOURCE=2 -floop-interchange -floop-strip-mine -floop-block")# -mavx -mxop -mfma4")

#Check C++14 standard support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++14 support.")
endif()

#Boost
FIND_PACKAGE(Boost REQUIRED)
IF(Boost_NOTFOUND)
	MESSAGE(FATAL_ERROR "Boost library not found")
ENDIF()

#Eigen
FIND_PACKAGE(Eigen3 REQUIRED)
IF(Eigen3_NOTFOUND)
	MESSAGE(FATAL_ERROR "Eigen3 library not found")
ENDIF()

#Find OpenMP
FIND_PACKAGE(OpenMP)
IF (OPENMP_FOUND)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -floop-parallelize-all -ftree-parallelize-loops=4")
ENDIF()


INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
ADD_EXECUTABLE(SimFault main.cpp)

SET_TARGET_PROPERTIES(SimFault PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
TARGET_LINK_LIBRARIES(SimFault LibSimFault)

