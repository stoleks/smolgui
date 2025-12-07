####
set (CONTENTS "${PROJECT_SOURCE_DIR}/contents")
set (OUTPUTS "${PROJECT_SOURCE_DIR}/build/bin")

# Build minimal demo
set (MIN_APP minimalDemo)
add_executable (${MIN_APP} minimalDemo.cpp)
target_link_libraries (${MIN_APP} PRIVATE SmolGui)
target_compile_features (${MIN_APP} PRIVATE cxx_std_17)
target_compile_definitions (${MIN_APP} PRIVATE ContentsDir="${CONTENTS}")
set_target_properties (${MIN_APP} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUTS}")

# Build small demo with serialization
set (SERIAL_APP serializationDemo)
add_executable (${SERIAL_APP} serializationDemo.cpp)
target_link_libraries (${SERIAL_APP} PRIVATE SmolGui)
target_compile_features (${SERIAL_APP} PRIVATE cxx_std_17)
target_compile_definitions (${SERIAL_APP} PRIVATE ContentsDir="${CONTENTS}")
set_target_properties (${SERIAL_APP} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUTS}")

# Build texture test
set (TEX_APP testTexture)
add_executable (${TEX_APP} testTexture.cpp)
target_link_libraries (${TEX_APP} PRIVATE SmolGui)
target_compile_features (${TEX_APP} PRIVATE cxx_std_17)
target_compile_definitions (${TEX_APP} PRIVATE ContentsDir="${CONTENTS}")
set_target_properties (${TEX_APP} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUTS}")
