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
set (DEMO_APP smolguiDemo)
add_executable (${DEMO_APP} main.cpp)

target_link_libraries (${DEMO_APP} PRIVATE
  SFML::Graphics
  SFML::Audio
  nlohmann_json
  spdlog
  SmolGui
)

target_compile_definitions (${DEMO_APP} PRIVATE ContentsDir="${PROJECT_SOURCE_DIR}/contents")
target_compile_features(${DEMO_APP} PRIVATE cxx_std_17)

set_target_properties(${DEMO_APP}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin/"
)

####
# Build texture test
set (TEX_APP testTexture)
add_executable (${TEX_APP} testTexture.cpp)

target_link_libraries (${TEX_APP} PRIVATE
  SFML::Graphics
  SFML::Audio
  nlohmann_json
  spdlog
  SmolGui
)

target_compile_definitions (${TEX_APP} PRIVATE ContentsDir="${PROJECT_SOURCE_DIR}/contents")
target_compile_features(${TEX_APP} PRIVATE cxx_std_17)

set_target_properties(${TEX_APP}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/bin/"
)
