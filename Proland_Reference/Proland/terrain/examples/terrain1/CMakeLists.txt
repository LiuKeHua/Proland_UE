



add_definitions(-D CURRENT_PROJECT_DIR=\\"${CMAKE_CURRENT_SOURCE_DIR}\\" )


SET(RES
	HelloWorld.xml
	upsampleShader.glsl
	terrainShader.glsl
	normalShader.glsl
	quad.mesh

)

source_group("Resources" FILES ${RES})


SET(TARGET_SRC 
	HelloWorld.cpp
	${RES}
)


VSP_ADD_EXE(proland_terrain_terrain1)


set_target_properties(proland_terrain_terrain1 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

