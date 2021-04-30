cmake_minimum_required(VERSION 3.12)
project( 
    RandomVariable
    VERSION 0.0.1
    LANGUAGES CXX
    )

  # make cache variables for install destinations
include(GNUInstallDirs)

# include CMakePackageConfigHelpers macro
include(CMakePackageConfigHelpers)

# include(CTest)
# enable_testing()

# Option to build examples
option(BUILD_EXAMPLES "Build all examples." OFF)

# Add (interface) library 
add_subdirectory(include)

# Add the header files
# target_include_directories( RandomVariable PUBLIC include)
if(BUILD_EXAMPLES)
    message(STATUS "Building example")
    add_subdirectory( examples)
endif()

########################################
# Installing headers
########################################
# generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/RandomVariableConfigVersion.cmake"
  VERSION ${CMAKE_PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/RandomVariableConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RandomVariable
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# install config files
install(FILES
          "${CMAKE_CURRENT_BINARY_DIR}/RandomVariableConfig.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/RandomVariableConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/RandomVariable
)

# generate the export targets for the build tree
export(EXPORT RandomVariableTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/RandomVariableTargets.cmake"
       NAMESPACE RandomVariable::
)