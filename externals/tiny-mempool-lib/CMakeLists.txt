cmake_minimum_required(VERSION 3.2)

project(tiny_mempool_lib C CXX)

#include(GNUInstallDirs)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(tiny_mempool_lib INTERFACE include/)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)