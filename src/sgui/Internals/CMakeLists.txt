####
# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Internals)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Internals)

# all source files
set (SRC
  ${SRCROOT}/ScrollerInformation.cpp
  ${INCROOT}/ScrollerInformation.h
  ${SRCROOT}/Counters.cpp
  ${INCROOT}/Counters.h
  ${INCROOT}/GuiStates.h
  ${INCROOT}/ItemStates.h
  ${INCROOT}/GuiGroup.h
)
source_group ("" FILES ${SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${SRC})
