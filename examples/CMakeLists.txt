cmake_minimum_required( VERSION 3.10)

project( example)

add_executable( ${PROJECT_NAME} example.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

find_package(Eigen3 REQUIRED)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIRS} "../src")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")