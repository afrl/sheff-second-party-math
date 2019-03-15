cmake_minimum_required(VERSION 3.8)

cmake_policy(SET CMP0048 NEW)
project(SECONDPARTYMATH VERSION 1.0.0 LANGUAGES CXX)

# Options
option(SECONDPARTYMATH_BuildDev "Build the project in the dev folder. (intended for debugging this project by devs)" OFF)
option(SECONDPARTYMATH_BuildTests "Build the unit tests." OFF)

# For debugging, print generator, compiler, and options
message(STATUS "Gen: ${CMAKE_GENERATOR}, CXX: ${CMAKE_CXX_COMPILER_ID}, Mode: ${CMAKE_BUILD_TYPE}")
message(STATUS "Dev: ${SECONDPARTYMATH_BuildDev}, Tests: ${SECONDPARTYMATH_BuildTests}")

##################
# Checking input #
##################

include(${CMAKE_SOURCE_DIR}/cmake/CommonInputChecks.cmake)

########################
# Declare dependencies #
########################




################################
# Setting up SECONDPARTYMATH target #
################################

# Set the variables for SECONDPARTYMATH
set(SECONDPARTYMATH_INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(SECONDPARTYMATH_INCLUDE_SUB ${SECONDPARTYMATH_INCLUDE}/second_party_math)
set(SECONDPARTYMATH_SRC ${CMAKE_SOURCE_DIR}/source)
file (GLOB_RECURSE SECONDPARTYMATH_HEADER_LIST "${SECONDPARTYMATH_INCLUDE}/*.h")
file (GLOB_RECURSE SECONDPARTYMATH_SOURCE_LIST "${SECONDPARTYMATH_SRC}/*.h" "${SECONDPARTYMATH_SRC}/*.cpp")
    
# Create target
add_library(secondpartymath STATIC)
# Add the header includes and headers to the library
target_include_directories(secondpartymath PUBLIC
    $<BUILD_INTERFACE:${SECONDPARTYMATH_INCLUDE}>
    $<INSTALL_INTERFACE:include>)
# Add headers in the include folder as PUBLIC so downstream
# targets have access to them
target_sources(secondpartymath PUBLIC ${SECONDPARTYMATH_HEADER_LIST})
# Add headers and sources in the source folder as PRIVATE as they
# will not be needed downstream
target_sources(secondpartymath PRIVATE ${SECONDPARTYMATH_SOURCE_LIST})
# Add an alias so that a library can be used inside the build tree, eg when testing
add_library(SecondPartyMath::SecondPartyMath ALIAS secondpartymath)
# Add C++ 14 flag
target_compile_features(secondpartymath PUBLIC cxx_std_14)
# Link dependencies

# Modify the properties for one source in particular due to the source file requiring too
# larger of an object on MSVC
if (MSVC)
    set_source_files_properties(${SECONDPARTYMATH_SRC}/ExprtkWrappers.cpp PROPERTIES COMPILE_FLAGS /bigobj)
endif()

# Assign source groups
include(${CMAKE_SOURCE_DIR}/cmake/Utilities.cmake)
assign_source_group("second_party_math" ${SECONDPARTYMATH_HEADER_LIST})
assign_source_group("second_party_math" ${SECONDPARTYMATH_SOURCE_LIST})


#################################
# Add dev and unit test targets #
#################################

# Build dev
if(SECONDPARTYMATH_BuildDev)
    add_subdirectory(dev)
endif()
    
# Build tests
if(SECONDPARTYMATH_BuildTests)
    include(CTest)
    enable_testing()
    add_subdirectory(unit_tests)
endif()

################
# Installation #
################

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/SecondPartyMath)

# Install the secondpartymath target (library and headers)
install(
    TARGETS secondpartymath
    EXPORT SecondPartyMathTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)
# This is required so that the exported target has the name SecondPartyMath and not secondpartymath
set_target_properties(secondpartymath PROPERTIES EXPORT_NAME SecondPartyMath)

# Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/SecondPartyMathConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Create the package config file for find_package(SecondPartyMath)
configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/SecondPartyMathConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SecondPartyMathConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

# Install the config and config version files
# Note: if you write custom find modules, add them here
# Refer to https://github.com/pabloariasal/modern-cmake-sample/blob/cfda7b51ff915409fd61d4f2e75b63032bedb182/libjsonutils/CMakeLists.txt
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/SecondPartyMathConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SecondPartyMathConfigVersion.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SecondPartyMathTargets.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

###########
# Exports #
###########

# Export just the secondpartymath target (not the dev, tests, etc.)
export(
    TARGETS secondpartymath
    FILE ${CMAKE_CURRENT_BINARY_DIR}/SecondPartyMathTargets.cmake
    NAMESPACE SecondPartyMath::
)

# Register package in user's package registry
export(PACKAGE SecondPartyMath)
