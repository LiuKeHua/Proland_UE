



SET(RES
	HelloWorld.xml
	cloud.mesh
	cloudShader.glsl
	globalsShader.glsl

	text.png
	textShader.glsl

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

VSP_ADD_EXE(proland_terrain_terrain5)

set_target_properties(proland_terrain_terrain5 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

