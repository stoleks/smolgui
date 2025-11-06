####
#include all subdirectories
add_subdirectory (sgui)

####
# Compilation option
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

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
