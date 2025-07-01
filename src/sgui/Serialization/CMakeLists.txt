# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Serialization)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Serialization)

# all source files
set (SERIALIZE_SRC
  ${INCROOT}/SerializeCore.h
  ${INCROOT}/SerializeGUI.h
  ${INCROOT}/SerializeSFML.h
)
source_group ("" FILES ${SERIALIZE_SRC})

set (LOAD_SRC
  ${INCROOT}/LoadLookupTable.h
  ${SRCROOT}/LoadJson.cpp
  ${INCROOT}/LoadJson.h
  ${SRCROOT}/LoadLayout.cpp
  ${INCROOT}/LoadLayout.h
  ${SRCROOT}/LoadTextureAtlas.cpp
  ${INCROOT}/LoadTextureAtlas.h
)
source_group ("load resources" FILES ${LOAD_SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${SERIALIZE_SRC} ${LOAD_SRC})
