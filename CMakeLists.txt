cmake_minimum_required(VERSION 3.5.1)
project(gazebo_plugin_tutorial)

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")


set(CMAKE_CXX_STANDARD 17)

add_library(visual_edit SHARED VisualEdit.cpp)
target_link_libraries(visual_edit ${GAZEBO_LIBRARIES})

include_directories(include)

#add_executable(gazebo_plugin_tutorial main.cpp)