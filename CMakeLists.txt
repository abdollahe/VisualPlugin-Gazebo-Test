cmake_minimum_required(VERSION 3.5.1)
project(gazebo_plugin_tutorial)

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")


set(CMAKE_CXX_STANDARD 14)

add_library(visual_edit SHARED VisualEdit.cpp)
target_link_libraries(visual_edit ${GAZEBO_LIBRARIES})

include_directories(include)

add_executable(ColorCom main.cpp)
add_executable(AmbCom AmbCom.cpp)
add_executable(App App.cpp)


if (${gazebo_VERSION_MAJOR} LESS 6)
    # These two
    include(FindBoost)
    find_package(Boost ${MIN_BOOST_VERSION} REQUIRED system filesystem regex)
    target_link_libraries(ColorCom ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
else()
    target_link_libraries(ColorCom ${GAZEBO_LIBRARIES})
    target_link_libraries(AmbCom ${GAZEBO_LIBRARIES})
    target_link_libraries(App ${GAZEBO_LIBRARIES})
endif()