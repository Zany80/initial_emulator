cmake_minimum_required(VERSION 3.0)

# Enable use of VERSION in project() command
cmake_policy(SET CMP0048 NEW)

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Default to static libs
if(NOT DEFINED SFML_STATIC_LIBRARIES)
	set(SFML_STATIC_LIBRARIES FALSE CACHE BOOL "Use static SFML libs" FORCE)
	set(SFML_ROOT ${CMAKE_SOURCE_DIR}/libs/)
endif()

set(TGUI_ROOT ${SFML_ROOT})

if(NOT ZENITH_SUPERDEBUGMODE)
	set(ZENITH_SUPERDEBUGMODE FALSE CACHE BOOL "Super debug mode (activates verbose debug logging to standard error)" FORCE)
endif()

project(ZENITH80 VERSION 0.1.0.0 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

include(git_revision)

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${ZENITH80_BINARY_DIR}" "${ZENITH80_SOURCE_DIR}/include")

set(EXECUTABLE_NAME zenith80)
add_subdirectory(src)
add_executable(${EXECUTABLE_NAME} ${SOURCES})

find_package(SFML 2 REQUIRED graphics window system )
if(SFML_NOTFOUND)
	message(FATAL_ERROR "SFML not found!")
endif()

find_package(TGUI 0.8 REQUIRED)
if(TGUI_NOTFOUND)
	message(FATAL_ERROR "TFUI not found!")
endif()

include_directories(${SFML_INCLUDE_DIR} ${TGUI_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${TGUI_LIBRARY} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR ${ZENITH80_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${ZENITH80_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${ZENITH80_VERSION_PATCH})
#set(CPACK_PACKAGE_EXECUTABLES ${executable_name} "icon")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "pixelherodev")
include(CPack)
