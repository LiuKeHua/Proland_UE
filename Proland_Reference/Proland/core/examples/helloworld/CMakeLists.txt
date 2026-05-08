


add_definitions("-DORK_STBI_API= __declspec(dllimport)")
add_definitions("-DORK_XML_API= __declspec(dllimport)")
add_definitions("-DORK_API= __declspec(dllimport)")
add_definitions("-DPROLAND_CORE_API=__declspec(dllimport)")


SET(RES
	helloworld.xml
	quad.mesh
	terrainShader.glsl
)
SET(TARGET_SRC 
	HelloWorld.cpp
	${RES}
)

source_group("Resources" FILES ${RES})

SET(LIB_INTERNAL_DEPS
	stbi
	tinyxml
	ork
	proland_core
)



VSP_ADD_EXE(proland_core_helloworld)

set_target_properties(proland_core_helloworld PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")


#foreach(r ${RES})
#	moveLib(proland_core_helloworld ${CMAKE_CURRENT_SOURCE_DIR}/${r} ${CMAKE_CURRENT_BINARY_DIR}/${r} )
#endforeach()



