####
#include all subdirectories
add_subdirectory (core)
add_subdirectory (serialization)
add_subdirectory (resources)
add_subdirectory (render)
add_subdirectory (parser)
add_subdirectory (gui)

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

####
# Build small demo
add_executable (Demo main.cpp)

target_link_libraries (Demo PRIVATE
  SFML::Graphics
  nlohmann_json
  spdlog
  SmolGui
)

target_compile_features(Demo PRIVATE cxx_std_17)

set_target_properties(Demo
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin/"
)
