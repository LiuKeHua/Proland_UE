

SET(RES
	helloworld.xml
	quad.mesh
	contourShader.glsl
	normalShader.glsl
	terrainShader.glsl
	
	upsampleOrthoShader.glsl
	upsampleShader.glsl

	textShader.glsl
	text.png
)

source_group("Resources" FILES ${RES})

SET(TARGET_SRC 
	HelloWorld.cpp
	${RES}
)

VSP_ADD_EXE(proland_terrain_exercise5)




set_target_properties(proland_terrain_exercise5 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

