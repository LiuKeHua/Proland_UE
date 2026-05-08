
file(GLOB SOURCE_FILES *.cpp *.h)


add_definitions("-DORK_STBI_API=__declspec(dllexport)")



SET(LIB_SOURCES 
	${SOURCE_FILES}
)


VSP_ADD_LIBRARY(stbi )
