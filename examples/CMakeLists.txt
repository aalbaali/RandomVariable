cmake_minimum_required( VERSION 3.10)

project( example)

add_executable( example example.cpp)
set_property(TARGET example PROPERTY CXX_STANDARD 17)

find_package(manif REQUIRED)
find_package(Eigen3 REQUIRED)
target_include_directories(example SYSTEM PRIVATE ${EIGEN3_INCLUDE_DIRS})
# Target to the RandomVariable interface "library"
target_link_libraries(example RandomVariable::RandomVariable)

set_property(TARGET example PROPERTY CXX_STANDARD 17)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
