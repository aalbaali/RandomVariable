add_library(RandomVariable INTERFACE)
# Create alias
add_library(RandomVariable::RandomVariable ALIAS RandomVariable)

find_package(Eigen3 REQUIRED)
target_link_libraries(RandomVariable PUBLIC INTERFACE Eigen3::Eigen)

# Add inlude directories
target_include_directories(
    RandomVariable
    INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/RandomVariable>"
)

set_target_properties(
    RandomVariable
    PROPERTIES
        CXX_STANDARD    11
)

# install the target and create export-set
install(TARGETS RandomVariable
        EXPORT RandomVariableTargets
        LIBRARY DESTINATION  ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION  ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION  ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/RandomVariable"
)

# install header file
install(FILES RV.h RVIO.h 
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/RandomVariable"
)

# # generate and install export file
# install(EXPORT RandomVariableTargets
#         FILE RandomVariableTargets.cmake
#         NAMESPACE RandomVariable::
#         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RandomVariable
# )