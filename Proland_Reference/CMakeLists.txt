cmake_minimum_required(VERSION 4.0.0)
project(MyProland)


SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on window")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows" )
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

#MinSizeRel;RelWithDebInfo
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;" CACHE STRING "possible configurations" FORCE)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/Common;${CMAKE_MODULE_PATH}")
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/version_utils;${CMAKE_MODULE_PATH}")


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")


SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

add_definitions(-D no_init_all=deprecated)

 
#include(generate_product_version)
#generate_product_version(
#   VersionFilesOutputDllInfo
#   NAME               "GISEngine"
#   BUNDLE             "GISEngine"
#   ICON               ""
#   VERSION_MAJOR       2
#   VERSION_MINOR       0
#   VERSION_PATCH       1
#   VERSION_REVISION    3
#   COMPANY_NAME        "xxx, Ltd"
#   COMPANY_COPYRIGHT   "xxx, Ltd. (C) Copyright 2019"
#   COMMENTS            "Parallel Rending v${VERSION_MAJOR}.${VERSION_MINOR}"
#   ORIGINAL_FILENAME   ${NAME}
#   INTERNAL_NAME       ${NAME} 
#   FILE_DESCRIPTION    ${NAME}
#)


INCLUDE(UtilityMacros)


FIND_PACKAGE(THIRDPARTY)



INCLUDE_DIRECTORIES(
	${THIRDPARTY_INCLUDE_DIR}
	${THIRDPARTY_INCLUDE_DIR}/tiff
	${CMAKE_CURRENT_SOURCE_DIR}

)


add_compile_options(/wd4312)
add_compile_options(/wd4251)
add_compile_options(/wd4244)
add_compile_options(/wd4996)
add_compile_options(/wd4267)
add_compile_options(/wd4305)
add_compile_options(/wd4845)
add_compile_options(/wd4311)
add_compile_options(/wd4302)



SET(MyCategory Default)

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/ork
	${CMAKE_CURRENT_SOURCE_DIR}/ork/libraries
)

option(BUILD_EXAMPLE      "Build shared library instead of static"   OFF)

add_definitions("-DTIXML_USE_STL")
add_definitions("-DHAVE_STRUCT_TIMESPEC")
#add_definitions("-DUSE_SHARED_PTR")
#add_definitions("-DUSEFREEGLUT")
#add_definitions("-DNDEBUG")


SET(LIB_EXTERNAL_DEPS
	GLEW_LIBRARY
	FREEGLUT_LIBRARY
	PTHREADS_LIBRARY
	AntTweakBar_LIBRARY
	TIFF_LIBRARY
)

add_subdirectory(ork)
add_subdirectory(Proland)


