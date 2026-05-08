

add_definitions("-D_CRT_SECURE_NO_WARNINGS")
add_definitions("-D_STDCALL_SUPPORTED")
add_definitions("-DGLUT_NO_LIB_PRAGMA")
add_definitions("-DGLUT_NO_WARNING_DISABLE")
add_definitions("-DSTBI_NO_WRITE")
add_definitions("-DSTBI_NO_STDIO")


add_definitions("-DORK_STBI_API=__declspec(dllimport)")
add_definitions("-DORK_XML_API=__declspec(dllimport)")
add_definitions("-DORK_API=__declspec(dllexport)")



 SET(Current_DIR ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE ALL_FILES "${Current_DIR}/*")
SET(ROOT_GROUP "ork")
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

SET(LIB_INTERNAL_DEPS
	stbi
	tinyxml
)



VSP_ADD_LIBRARY(ork )




