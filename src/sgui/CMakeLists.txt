####
#include all subdirectories
add_subdirectory (Core)
add_subdirectory (Parser)
add_subdirectory (Render)
add_subdirectory (Resources)
add_subdirectory (Serialization)
add_subdirectory (Widgets)

####
# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/sgui)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sgui)

# all source files
set (SRC
  ${SRCROOT}/Gui.cpp
  ${INCROOT}/Gui.h
  ${INCROOT}/Gui.tpp
)
source_group ("" FILES ${SRC})

set (UI_SRC
  ${INCROOT}/sgui.h
)
source_group ("user interface" FILES ${UI_SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${SRC} ${UI_SRC})
