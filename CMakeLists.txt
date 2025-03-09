####
# Cmake configuration
cmake_minimum_required (VERSION 3.28)
set (CMAKE_CONFIGURATION_TYPES "Debug;Release")

####
# Projects informations
project (SmolGui
  VERSION 0.1.0
  DESCRIPTION "Smol gui library that use immediate gui principles."
  LANGUAGES CXX
)
set (PROJECT_AUTHOR "Alexandre Jedrecy")
set (PROJECT_IDENTIFIER "stoleks.sgui")

####
# Third party libraries, SFML for input/rendering, json for serialization, spdlog for logging
include (FetchContent)

FetchContent_Declare (SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.0.0
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)

FetchContent_Declare (nlohmann-json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
  GIT_PROGRESS TRUE
)

FetchContent_Declare (spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.9.2
  GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable (
  SFML
  nlohmann-json
  spdlog
)

####
# set library output directory
set (LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# add sgui library
add_library(${PROJECT_NAME})
add_library(SmolGui::${PROJECT_NAME} ALIAS SmolGui)

####
# Add src as public directory
target_include_directories (${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")

####
# get sources from subdirectories
add_subdirectory (src)
