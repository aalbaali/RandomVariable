cmake_minimum_required( VERSION 3.10)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++11")

project( example)

add_executable( ${PROJECT_NAME} example.cpp)

find_package(Eigen3 REQUIRED)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${EIGEN3_INCLUDE_DIRS})

# Set required C++11 flag
if(CMAKE_VERSION VERSION_LESS "3.1")
  set_target_properties(${CXX_11_EXAMPLE_TARGETS} PROPERTIES COMPILE_FLAGS "-std=c++11")
else()
  set_property(TARGET ${CXX_11_EXAMPLE_TARGETS} PROPERTY CXX_STANDARD 11)
  set_property(TARGET ${CXX_11_EXAMPLE_TARGETS} PROPERTY CXX_STANDARD_REQUIRED ON)
  set_property(TARGET ${CXX_11_EXAMPLE_TARGETS} PROPERTY CXX_EXTENSIONS OFF)
endif()
