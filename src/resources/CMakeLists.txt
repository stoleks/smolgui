# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/resources)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/resources)

# all source files
set (LOAD_SRC
  ${SRCROOT}/LoadLayout.cpp
  ${INCROOT}/LoadLayout.h
  ${INCROOT}/LoadLookupTable.tpp
  ${INCROOT}/LoadLookupTable.h
  ${SRCROOT}/LoadTextureAtlas.cpp
  ${INCROOT}/LoadTextureAtlas.h
)
source_group ("load resources" FILES ${LOAD_SRC})

set (SRC
  ${SRCROOT}/Layout.cpp
  ${INCROOT}/Layout.h
  ${INCROOT}/Layout.tpp
  ${SRCROOT}/TextContainer.cpp
  ${INCROOT}/TextContainer.h
  ${SRCROOT}/TextureAtlas.cpp
  ${INCROOT}/TextureAtlas.h
)
source_group ("" FILES ${SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${SRC} ${LOAD_SRC})
