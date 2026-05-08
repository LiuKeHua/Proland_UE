

SET(MyCategory proland)



INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/core/sources
	${CMAKE_CURRENT_SOURCE_DIR}/atmo/sources
	${CMAKE_CURRENT_SOURCE_DIR}/edit/sources
	${CMAKE_CURRENT_SOURCE_DIR}/forest/sources
	${CMAKE_CURRENT_SOURCE_DIR}/graph/sources
	${CMAKE_CURRENT_SOURCE_DIR}/ocean/sources
	${CMAKE_CURRENT_SOURCE_DIR}/terrain/sources
	${CMAKE_CURRENT_SOURCE_DIR}/river/sources

)





add_subdirectory(core)

add_subdirectory(terrain)

#add_subdirectory(demo)

