



SET(RES
	helloworld.xml
	quad.mesh
	bluemarble.png
	srtm.png

	text.png
	textShader.glsl


	normalShader.glsl
	terrainShader.glsl

	upsampleOrthoShader.glsl
	upsampleShader.glsl
)

source_group("Resources" FILES ${RES})



SET(TARGET_SRC 
	HelloWorld.cpp
	${RES}
)

VSP_ADD_EXE(proland_terrain_preprocess)



set_target_properties(proland_terrain_preprocess PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")


