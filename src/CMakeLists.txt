####
#include all subdirectories
add_subdirectory (sgui)

####
# Compilation option
set (CONTENTS "${PROJECT_SOURCE_DIR}/contents")
target_compile_features (${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_definitions (${PROJECT_NAME} PUBLIC ContentsDir="${CONTENTS}")

####
# Link third party libraries
target_link_libraries (${PROJECT_NAME}
  PUBLIC
    SFML::Graphics
    nlohmann_json
    spdlog
  PRIVATE
    SFML::Audio
)
