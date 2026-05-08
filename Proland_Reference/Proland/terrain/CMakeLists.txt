SET(MyCategory proland/terrain)


add_subdirectory(sources)

IF(BUILD_EXAMPLE)
add_subdirectory(examples)
ENDIF()
