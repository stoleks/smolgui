# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Resources)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Resources)

# all source files
set (LOAD_SRC
  ${SRCROOT}/LoadLayout.cpp
  ${INCROOT}/LoadLayout.h
  ${INCROOT}/LoadLookupTable.h
  ${SRCROOT}/LoadTextureAtlas.cpp
  ${INCROOT}/LoadTextureAtlas.h
)
source_group ("load resources" FILES ${LOAD_SRC})

set (SRC
  ${SRCROOT}/Layout.cpp
  ${INCROOT}/Layout.h
  ${INCROOT}/Layout.tpp
  ${INCROOT}/ResourcesHolder.h
  ${INCROOT}/ResourcesHolder.tpp
  ${SRCROOT}/SoundPlayer.cpp
  ${INCROOT}/SoundPlayer.h
  ${SRCROOT}/TextContainer.cpp
  ${INCROOT}/TextContainer.h
  ${SRCROOT}/TextureAtlas.cpp
  ${INCROOT}/TextureAtlas.h
)
source_group ("" FILES ${SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${SRC} ${LOAD_SRC})
