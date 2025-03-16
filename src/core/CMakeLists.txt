set(INCROOT ${PROJECT_SOURCE_DIR}/src/core)

# all source files
set (MATH_SRC
  ${INCROOT}/Interpolation.tpp
  ${INCROOT}/Interpolation.h
)
source_group ("" FILES ${MATH_SRC})

set (CONTAINER_SRC
  ${INCROOT}/LookupTable.tpp
  ${INCROOT}/LookupTable.h
  ${INCROOT}/ObjectPool.tpp
  ${INCROOT}/ObjectPool.h
)
source_group ("container" FILES ${CONTAINER_SRC})

set (SHAPES_SRC
  ${INCROOT}/Polygon.tpp
  ${INCROOT}/Polygon.h
  ${INCROOT}/Shapes.tpp
  ${INCROOT}/Shapes.h
)
source_group ("shapes" FILES ${SHAPES_SRC})

target_sources (${PROJECT_NAME}
  PRIVATE ${MATH_SRC} ${CONTAINER_SRC} ${SHAPES_SRC})
