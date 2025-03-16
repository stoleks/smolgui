set(INCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Parser)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/sgui/Parser)

# all source files
set (MATH_SRC
  ${INCROOT}/MathParser.tpp
  ${INCROOT}/MathParser.h
  ${SRCROOT}/MathParser.cpp
  ${INCROOT}/Formula.h
)
source_group ("math parser" FILES ${MATH_SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${MATH_SRC})
