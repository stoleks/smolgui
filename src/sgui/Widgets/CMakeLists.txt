####
# if anyday I want to separate src and include...
set(INCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Widgets)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Widgets)

# all source files
set (SRC
  ${INCROOT}/Panel.h
  ${INCROOT}/Options.h
  ${INCROOT}/Constraints.h
  ${INCROOT}/Style.h
  ${INCROOT}/Widgets.h
)
source_group ("" FILES ${SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${SRC})
