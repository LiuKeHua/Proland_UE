
SET(RES
	helloworld.xml
	drawPlantsMethod.xml
	drawPlantsShadowMethod.xml
	quad.mesh
	normalShader.glsl
	terrainShader.glsl
	text.png
	textShader.glsl
	textureTile.glsl

	upsampleOrthoShader.glsl
	upsampleShader.glsl
	upsampleLccShader.glsl

	elevationFilter.glsl
	initOrthoShader.glsl
	renderTreeShader.glsl

	selectTreeShader.glsl
	slopeFilter.glsl

	basictree.png
	brushOrthoShader.glsl
	composeOrthoShader.glsl
	copyShader.glsl
)

source_group("Resources" FILES ${RES})



SET(TARGET_SRC 
	HelloWorld.cpp
	${RES}
)

VSP_ADD_EXE(proland_terrain_exercise2)


set_target_properties(proland_terrain_exercise2 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")


