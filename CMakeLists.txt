cmake_minimum_required(VERSION 3.10)
project( 
    RandomVariable
    VERSION 0.0.1
    LANGUAGES CXX)

# include(CTest)
# enable_testing()

# Option to build examples
option(BUILD_EXAMPLES "Build all examples." ON)

add_library(${PROJECT_NAME} INTERFACE)
# target_include_directories( 
#     ${PROJECT_NAME} 
#     INTERFACE
#     "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include"
#     "$<INSTALL_INTERFACE:include>"
#     )
target_include_directories( 
    ${PROJECT_NAME} 
    INTERFACE
    include/RandomVariable
    )

# Add the header files
# target_include_directories( RandomVariable PUBLIC include)
add_subdirectory( examples)