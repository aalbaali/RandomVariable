cmake_minimum_required(VERSION 3.10)
project( 
    RandomVariable
    VERSION 0.0.1
    LANGUAGES CXX
    )

# include(CTest)
# enable_testing()

# Option to build examples
option(BUILD_EXAMPLES "Build all examples." OFF)

# Find package
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(RandomVariable)

# Add the header files
# target_include_directories( RandomVariable PUBLIC include)
if(BUILD_EXAMPLES)
    message(STATUS "Building example")
    add_subdirectory( examples)
endif()


########################################
# Installing headers
########################################
install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}"
  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/include"
#   DESTINATION /usr/local/include
)