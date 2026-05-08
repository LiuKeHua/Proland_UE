


SET(RES
	helloworld.xml
	quad.mesh
	square.mesh
	globalsShader.glsl
	atmosphereShader.glhl
	atmosphereShader.glsl
	normalShader.glsl
	terrainShader.glsl
	oceanShader.glsl
	skyShader.glsl
	text.png
	textShader.glsl
	sunglare.png
	upsampleOrthoShader.glsl
	upsampleShader.glsl

)

source_group("Resources" FILES ${RES})



SET(TARGET_SRC 
	HelloWorld.cpp
	${RES}
)

VSP_ADD_EXE(proland_terrain_exercise1)


set_target_properties(proland_terrain_exercise1 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
