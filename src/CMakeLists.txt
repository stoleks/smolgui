add_executable(${PROJECT_NAME})

####
#include all subdirectories
add_subdirectory (core)
add_subdirectory (serialization)
add_subdirectory (resources)
add_subdirectory (render)
add_subdirectory (gui)

####
# Add source files
target_sources (${PROJECT_NAME} PRIVATE main.cpp)

####
# Add src as public directory
target_include_directories (${PROJECT_NAME}
  PUBLIC "${PROJECT_SOURCE_DIR}/src"
)

####
# Compilation option
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

####
# Link third party libraries
target_link_libraries (${PROJECT_NAME} PRIVATE
  SFML::Graphics
  nlohmann_json
  spdlog
)

