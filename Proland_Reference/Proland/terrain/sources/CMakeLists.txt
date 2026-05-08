
add_definitions("-DORK_STBI_API= __declspec(dllimport)")
add_definitions("-DORK_XML_API= __declspec(dllimport)")
add_definitions("-DORK_API= __declspec(dllimport)")
add_definitions("-DPROLAND_CORE_API=__declspec(dllimport)")

add_definitions("-DPROLAND_TERRAIN_API=__declspec(dllexport)")


SET(LIB_INTERNAL_DEPS
	stbi
	tinyxml
	ork
	proland_core
)


SET(Current_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proland)
file(GLOB_RECURSE ALL_FILES "${Current_DIR}/*")
SET(ROOT_GROUP "proland_core")
FOREACH(child ${ALL_FILES})
       get_filename_component(FileDir  ${child} DIRECTORY )
       IF(${FileDir}  STREQUAL  ${Current_DIR})
              source_group("${ROOT_GROUP}" FILES ${child})
       ELSE()
              file(RELATIVE_PATH RelativeName ${Current_DIR} ${FileDir})
              string(REPLACE "/"  "\\\\"    Hello  ${RelativeName})
              source_group("${ROOT_GROUP}\\${Hello}" FILES ${child})
       ENDIF()
ENDFOREACH()


SET(LIB_SOURCES 
	${ALL_FILES}
)



VSP_ADD_LIBRARY(proland_terrain )




