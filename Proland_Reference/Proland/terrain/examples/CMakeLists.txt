

add_definitions("-DORK_STBI_API= __declspec(dllimport)")
add_definitions("-DORK_XML_API= __declspec(dllimport)")
add_definitions("-DORK_API= __declspec(dllimport)")
add_definitions("-DPROLAND_CORE_API=__declspec(dllimport)")
add_definitions("-DPROLAND_RIVER_API=__declspec(dllimport)")
add_definitions("-DPROLAND_TERRAIN_API=__declspec(dllimport)")
add_definitions("-DPROLAND_GRAPH_API=__declspec(dllimport)")
add_definitions("-DPROLAND_OCEAN_API=__declspec(dllimport)")
add_definitions("-DPROLAND_ATMO_API=__declspec(dllimport)")
add_definitions("-DPROLAND_EDIT_API=__declspec(dllimport)")
add_definitions("-DPROLAND_FOREST_API=__declspec(dllimport)")

 
SET(LIB_INTERNAL_DEPS
	stbi
	tinyxml
	ork
	proland_core

	proland_terrain

)



file(GLOB ALL_FILES "*")
FOREACH(child ${ALL_FILES})
	if(IS_DIRECTORY ${child})
		add_subdirectory(${child})
	ENDIF()
ENDFOREACH()

