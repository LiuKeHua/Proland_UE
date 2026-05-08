
SET(MyCategory proland/core)

add_subdirectory(sources)
IF(BUILD_EXAMPLE)
add_subdirectory(examples/helloworld)
ENDIF()


