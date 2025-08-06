# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Render)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Render)

# all source files
set (SRC
  ${SRCROOT}/GuiRender.cpp
  ${INCROOT}/GuiRender.h
  ${INCROOT}/GuiRender.tpp
  ${SRCROOT}/Plotter.cpp
  ${INCROOT}/Plotter.h
  ${SRCROOT}/PrimitiveShapeRender.cpp
  ${INCROOT}/PrimitiveShapeRender.tpp
  ${INCROOT}/PrimitiveShapeRender.h
)
source_group ("" FILES ${SRC})

set (HELPER_SRC
  ${SRCROOT}/MeshFunctions.cpp
  ${INCROOT}/MeshFunctions.h
  ${SRCROOT}/TextureMeshes.cpp
  ${INCROOT}/TextureMeshes.h
)
source_group ("drawable helper" FILES ${HELPER_SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${SRC} ${HELPER_SRC})
