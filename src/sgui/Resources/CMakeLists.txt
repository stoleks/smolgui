# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Resources)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Resources)

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
  ${SRCROOT}/TextureCollage.cpp
  ${INCROOT}/TextureCollage.h
)
source_group ("" FILES ${SRC})

target_sources (${PROJECT_NAME} PRIVATE ${SRC})
