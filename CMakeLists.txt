#  SuperTux Launcher
#  Copyright (C) 2024 Vankata453
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5)

# Initialize project

project(supertux-launcher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR})

# Copy data folders

file(GLOB DATA_SUBDIRS LIST_DIRECTORIES true ${PROJECT_SOURCE_DIR}/data/*)
list(FILTER DATA_SUBDIRS EXCLUDE REGEX "versions")

file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
file(COPY ${DATA_SUBDIRS} DESTINATION ${CMAKE_BINARY_DIR}/data)

## Versions
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data/versions)
if(WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
    set(TARGET_VERSIONS_DIR "win64")
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32-bit
    set(TARGET_VERSIONS_DIR "win32")
  else()
    message(FATAL "Unsupported Windows architecture!")
  endif()
elseif(LINUX)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8) # 64-bit
    set(TARGET_VERSIONS_DIR "linux64")
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4) # 32-bit
    set(TARGET_VERSIONS_DIR "linux32")
  else()
    message(FATAL "Unsupported Linux architecture!")
  endif()
else()
  message(FATAL "Unsupported operating system!")
endif()
file(COPY ${PROJECT_SOURCE_DIR}/data/versions/${TARGET_VERSIONS_DIR}/ DESTINATION ${CMAKE_BINARY_DIR}/data/versions)

# Get Git hash

find_package(Git)
if(GIT_FOUND)
  execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
                  RESULT_VARIABLE PACKAGE_BUILD_HASH_RESULT
                  OUTPUT_VARIABLE PACKAGE_BUILD_HASH
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(PACKAGE_BUILD_HASH_RESULT)
    message(WARNING "Failed to get Git hash: ${PACKAGE_BUILD_HASH_RESULT}")
  endif()
endif()

configure_file(${PROJECT_SOURCE_DIR}/version.h.in ${CMAKE_BINARY_DIR}/version.h)

# Find packages

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(CURL REQUIRED)

# Library include directories

include_directories(${CURL_INCLUDE_DIR})

# Import libraries

set(USE_CXX17 ON)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/sexp-cpp)

# Add executable

file(GLOB STL_SOURCE ${PROJECT_SOURCE_DIR}/src/*.cpp
                     ${PROJECT_SOURCE_DIR}/src/dialog/*.cpp
                     ${PROJECT_SOURCE_DIR}/src/handler/*.cpp
                     ${PROJECT_SOURCE_DIR}/src/install_method/*.cpp
                     ${PROJECT_SOURCE_DIR}/src/instance/*.cpp
                     ${PROJECT_SOURCE_DIR}/src/util/*.cpp
                     ${PROJECT_SOURCE_DIR}/src/version/*.cpp
                     ${PROJECT_SOURCE_DIR}/src/widget/*.cpp
                     ${PROJECT_SOURCE_DIR}/src/widget/element/*.cpp
                     ${PROJECT_SOURCE_DIR}/src/window/*.cpp)
add_executable(supertux-launcher ${STL_SOURCE})

# Link libraries

target_link_libraries(supertux-launcher PUBLIC Qt5::Core)
target_link_libraries(supertux-launcher PUBLIC Qt5::Widgets)
target_link_libraries(supertux-launcher PUBLIC sexp)
target_link_libraries(supertux-launcher PUBLIC ${CURL_LIBRARIES})
