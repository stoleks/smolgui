# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/gui)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/gui)

# all source files
set (SRC
  ${SRCROOT}/Gui.cpp
  ${INCROOT}/Gui.h
  ${INCROOT}/Gui.tpp
  ${SRCROOT}/ScrollerInformation.cpp
  ${INCROOT}/ScrollerInformation.h
  ${SRCROOT}/Style.cpp
  ${INCROOT}/Style.h
  ${INCROOT}/Widgets.h
)
source_group ("" FILES ${SRC})

set (UI_SRC
  ${INCROOT}/sgui.h
  ${SRCROOT}/UserInterface.cpp
  ${INCROOT}/UserInterface.h
)
source_group ("user interface" FILES ${UI_SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${SRC} ${UI_SRC})
