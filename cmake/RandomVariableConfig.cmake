# Return if target already defined
if( TARGET RandomVariable::RandomVariable)
    return()
endif()

add_library(RandomVariable::RandomVariable INTERFACE IMPORTED)
# Get parent path (project_path)
# New in version 3.20: 
#   https://cmake.org/cmake/help/latest/command/cmake_path.html#command:cmake_path
if( CMAKE_VERSION VERSION_GREATER_EQUAL 3.20)
    message(DEBUG "Using 'cmake_path' to extract parent directory")
    cmake_path( GET         CMAKE_CURRENT_LIST_DIR 
                PARENT_PATH CMAKE_PARENT_CURRENT_LIST_DIR)
else()                
    get_filename_component(CMAKE_PARENT_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
endif()
            
target_include_directories(
    RandomVariable::RandomVariable
    INTERFACE 
    "${CMAKE_PARENT_CURRENT_LIST_DIR}/include/RandomVariable"
)

set_target_properties(
    RandomVariable::RandomVariable
    PROPERTIES
        CXX_STANDARD    11
    )

# Display message
if(NOT DEFINED RandomVariable_FIND_QUIETLY)
    message( STATUS "Found RandomVariable: ${CMAKE_SOURCE_DIR}/include/RandomVariable")
endif()