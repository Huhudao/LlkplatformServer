cmake_minimum_required (VERSION 2.8)

project(LlkplatformServer)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++11")

aux_source_directory(./tools srcTools)
aux_source_directory(./network srcNetwork)
aux_source_directory(./processor srcData)
aux_source_directory(./server srcServer)

add_executable(llkserver ${srcTools} ${srcNetwork} ${srcData} ${srcServer})

TARGET_LINK_LIBRARIES(llkserver pthread mysqlclient)
