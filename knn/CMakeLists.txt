cmake_minimum_required(VERSION 3.2)
project(CppProjects VERSION 1.0.0.0 LANGUAGES CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake ${PROJECT_BINARY_DIR}/external/lib/cmake)

include(ExternalProject)
find_package(Git REQUIRED)
find_package(Eigen3 REQUIRED)


ExternalProject_Add(
  google-benchmark
  GIT_REPOSITORY      https://github.com/google/benchmark.git
  GIT_TAG             v1.3.0
  CMAKE_ARGS          "-DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/external;-DCMAKE_BUILD_TYPE=Release;-DBENCHMARK_ENABLE_TESTING=false"
  PREFIX              ${PROJECT_BINARY_DIR}/external/benchmark/prefix
  TMP_DIR             ${PROJECT_BINARY_DIR}/external/benchmark/tmp
  STAMP_DIR           ${PROJECT_BINARY_DIR}/external/benchmark/stamp
  DOWNLOAD_DIR        ${PROJECT_BINARY_DIR}/external/benchmark/download
  SOURCE_DIR          ${PROJECT_BINARY_DIR}/external/benchmark/src
  BINARY_DIR          ${PROJECT_BINARY_DIR}/external/benchmark/build
)

set(LIB_NAME libcm)

add_library(${LIB_NAME} STATIC
            "${CMAKE_SOURCE_DIR}/src/knn.h"
            "${CMAKE_SOURCE_DIR}/src/knn.cpp")
  
add_library(Cm::lib ALIAS ${LIB_NAME})

target_compile_features(${LIB_NAME}
                        PUBLIC cxx_std_17
                        PRIVATE cxx_lambdas cxx_nullptr)
  
target_include_directories(${LIB_NAME} SYSTEM
                           PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/external/include ${EIGEN3_INCLUDE_DIRS}> )
target_include_directories(${LIB_NAME}
                           PUBLIC  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src> $<INSTALL_INTERFACE:/include>)
target_compile_options(${LIB_NAME} PRIVATE $<$<CXX_COMPILER_ID:Clang>:-pedantic -Weverything -Wno-c++98-compat>)
target_compile_options(${LIB_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-pedantic -Wextra -Wall -Wno-c++98-compat>)

set(EXE_NAME cm)
add_executable(${EXE_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${EXE_NAME} Cm::lib)

find_package(Threads REQUIRED)

#compile file to benchmark
add_executable(cm_bench ${PROJECT_SOURCE_DIR}/src/bench.cpp)

target_compile_features(cm_bench 
                        PUBLIC cxx_std_17)
						
target_include_directories(cm_bench  
                           PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/external/include>)
						   
target_link_libraries(cm_bench ${PROJECT_BINARY_DIR}/external/lib/libbenchmark.a ${CMAKE_THREAD_LIBS_INIT})
add_dependencies(cm_bench google-benchmark)