

SET(RES
	helloworld.xml
	quad.mesh
	copyShader.glsl
	normalShader.glsl
	terrainShader.glsl
	text.png
	textShader.glsl

	upsampleOrthoShader.glsl
	upsampleShader.glsl

)
source_group("Resources" FILES ${RES})


SET(TARGET_SRC 
	HelloWorld.cpp

	${RES}
)

VSP_ADD_EXE(proland_terrain_exercise4)


set_target_properties(proland_terrain_exercise4 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")





