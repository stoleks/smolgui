# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/serialization)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/serialization)

# all source files
set (SRC
  ${SRCROOT}/LoadJson.cpp
  ${INCROOT}/LoadJson.h
  ${INCROOT}/SerializeCore.h
  ${SRCROOT}/SerializeGUI.cpp
  ${INCROOT}/SerializeGUI.h
  ${SRCROOT}/SerializeSFML.cpp
  ${INCROOT}/SerializeSFML.h
)
source_group ("" FILES ${SRC})

target_sources (${PROJECT_NAME} PRIVATE ${SRC})
