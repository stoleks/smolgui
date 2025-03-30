####
#include all subdirectories
add_subdirectory (sgui)

####
# Compilation option
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

####
# Link third party libraries
target_link_libraries (${PROJECT_NAME} PRIVATE
  SFML::Graphics
  SFML::Audio
  nlohmann_json
  spdlog
)

####
# Build small demo
add_executable (SmolguiDemo main.cpp)

target_link_libraries (SmolguiDemo PRIVATE
  SFML::Graphics
  SFML::Audio
  nlohmann_json
  spdlog
  SmolGui
)

target_compile_features(SmolguiDemo PRIVATE cxx_std_17)

set_target_properties(SmolguiDemo
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin/"
)
