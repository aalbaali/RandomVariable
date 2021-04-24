# Return if target already defined
if( TARGET RandomVariable::RandomVariable)
    return()
endif()

add_library(RandomVariable::RandomVariable INTERFACE IMPORTED)
target_include_directories(
    RandomVariable::RandomVariable
    INTERFACE 
    "${CMAKE_SOURCE_DIR}/include/RandomVariable"
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