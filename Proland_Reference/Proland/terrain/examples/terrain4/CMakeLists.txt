


SET(RES
	HelloWorld.xml
	upsampleShader.glsl
	terrainShader.glsl
	normalShader.glsl
	quad.mesh
	DEM.dat

	text.png
	textShader.glsl

)
source_group("Resources" FILES ${RES})


SET(TARGET_SRC 
	HelloWorld.cpp
	${RES}
)

VSP_ADD_EXE(proland_terrain_terrain4)


set_target_properties(proland_terrain_terrain4 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

