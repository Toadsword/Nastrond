cmake_minimum_required(VERSION 3.3)
project(SFGE)

include(cmake/IntelCompileFeatures.cmake)

include(cmake/EnumeratedOption.cmake)
include(cmake/CheckCXXFeature.cmake)
include(cmake/CheckCXXStandardLibrary.cmake)

message(STATUS "CXX compiler ID: '${CMAKE_CXX_COMPILER_ID}'")
message(STATUS "CXX compiler Version: '${CMAKE_CXX_COMPILER_VERSION}'")

enum_option(BUILD_CXX_STANDARD
  DOC "C++ Standard to compile against"
  VALUES 11 14 17
  CASE_INSENSITIVE
  )
message(STATUS "List of CXX features known to CMake")
	get_property(_cxx_known_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
	message(STATUS "List of supported CXX11 features")
	foreach(_f ${CMAKE_CXX11_COMPILE_FEATURES})
	  message("  ${_f}")
	endforeach()
	message(STATUS "List of supported CXX14 features")
	foreach(_f ${CMAKE_CXX14_COMPILE_FEATURES})
	  message("  ${_f}")
	endforeach()
	message(STATUS "List of supported CXX17 features")
	foreach(_f ${CMAKE_CXX17_COMPILE_FEATURES})
		message("  ${_f}")
endforeach()

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals)

include_directories(include ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SFGE_AUDIO_SRC src/audio/*.cpp include/audio/*.h)
file(GLOB_RECURSE SFGE_ENGINE_SRC src/engine/*.cpp include/engine/*.h)
file(GLOB_RECURSE SFGE_GRAPHICS_SRC src/graphics/*.cpp include/graphics/*.h)
file(GLOB_RECURSE SFGE_INPUT_SRC src/input/*.cpp include/input/*.h)
file(GLOB_RECURSE SFGE_PHYSICS_SRC src/physics/*.cpp include/physics/*.h)
file(GLOB_RECURSE SFGE_PYTHON_SRC src/python/*.cpp include/python/*.h)
file(GLOB_RECURSE SFGE_UTILITY_SRC src/utility/*.cpp include/utility/*.h)

set_property(GLOBAL PROPERTY USE_FOLDERS On)
SET_SOURCE_FILES_PROPERTIES( ${SFGE_SRC} PROPERTIES
                FOLDER "${CMAKE_SOURCE_DIR}/src" )

source_group("Audio"            FILES ${SFGE_AUDIO_SRC})
source_group("Engine"            FILES ${SFGE_ENGINE_SRC})
source_group("Graphics"            FILES ${SFGE_GRAPHICS_SRC})
source_group("Input"            FILES ${SFGE_INPUT_SRC})
source_group("Physics"            FILES ${SFGE_PHYSICS_SRC})
source_group("Python"            FILES ${SFGE_PYTHON_SRC})
source_group("Utility"            FILES ${SFGE_UTILITY_SRC})
List(APPEND SFGE_SRC  ${SFGE_AUDIO_SRC} ${SFGE_ENGINE_SRC} ${SFGE_GRAPHICS_SRC} 
${SFGE_INPUT_SRC} ${SFGE_PHYSICS_SRC}  ${SFGE_PYTHON_SRC} ${SFGE_UTILITY_SRC})
#SFML
set(SFML_DIR ${EXTERNAL_DIR}/SFML)
set(SFML_BUILD_EXAMPLES FALSE)
add_subdirectory(${SFML_DIR})
include_directories(${SFML_DIR}/include ${SFML_DIR}/src)
LIST(APPEND SFGE_LIBRARIES
	sfml-system
	sfml-window
	sfml-graphics
	sfml-audio
	sfml-network)

#SFML Imgui
set(SFML_IMGUI ${EXTERNAL_DIR}/imgui-sfml/)
set(IMGUI_ROOT ${EXTERNAL_DIR}/imgui/)

set(SFML_ROOT ${EXTERNAL_DIR}/SFML/)
add_subdirectory(${SFML_IMGUI})
include_directories(${IMGUI_ROOT} ${SFML_IMGUI})
LIST(APPEND SFGE_LIBRARIES
	imgui-sfml)


#BOX2D
set(BOX2D_DIR ${EXTERNAL_DIR}/Box2D)

SET(BOX2D_INSTALL ON CACHE INTERNAL "")
SET(BOX2D_BUILD_STATIC ON  CACHE INTERNAL "")

add_subdirectory(${BOX2D_DIR})
include_directories(${BOX2D_DIR}/Box2D/)
LIST(APPEND SFGE_LIBRARIES
	Box2D)

#PYTHON
#IF(NOT WIN32)
#	add_definitions(-Wall -Wextra -pedantic -std=c++11)
#
#	IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
#		add_definitions(-g -O0)
#	ELSE()
#		add_definitions(-O3)
#	ENDIF()
#
#	LIST(APPEND SFGE_LIBRARIES resolv)
#ELSE()
#	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
#ENDIF()


#find_package(PythonLibs 3 EXACT REQUIRED)

#include_directories(${PYTHON_INCLUDE_DIR})

#LIST(APPEND SFGE_LIBRARIES ${PYTHON_LIBRARY})

#PYBIND11
set(PYBIND11_PYTHON_VERSION 3)
set(PYBIND11_DIR ${EXTERNAL_DIR}/pybind11/)
add_subdirectory(${PYBIND11_DIR})
include_directories(${PYBIND11_DIR})
LIST(APPEND SFGE_LIBRARIES
  pybind11::embed)

if(APPLE)
	set(boost_min_ver 1.50.0)
	set(boost_libs system filesystem)
	find_package(Boost ${boost_min_ver})

	if(Boost_FOUND)
		find_package(Boost ${boost_min_ver} COMPONENTS ${boost_libs})
	endif()
	include_directories(${Boost_INCLUDE_DIRS})
	LIST(APPEND SFGE_LIBRARIES
	  ${Boost_LIBRARIES})
endif(APPLE)

#MISCELLANEOUS UTILITIES
set(UTILITIES_DIR ${EXTERNAL_DIR}/utilities/)
include_directories(${UTILITIES_DIR})


#SFGE_COMMON
if(NOT WIN32 AND NOT APPLE)
	LIST(APPEND SFGE_LIBRARIES
		stdc++fs)
ENDIF()
add_library(SFGE_COMMON STATIC ${SFGE_SRC})
include_directories(include/)
target_link_libraries(SFGE_COMMON PUBLIC ${SFGE_LIBRARIES})
set_property(TARGET SFGE_COMMON PROPERTY CXX_STANDARD 17)
if(APPLE)
set_target_properties(SFGE_COMMON PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
ENDIF(APPLE)

#SFGE_TEST
SET(SFGE_TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
file(GLOB TEST_FILES ${SFGE_TEST_DIR}/*.cpp )
foreach(test_file ${TEST_FILES} )
    # I used a simple string replace, to cut off .cpp.
    file(RELATIVE_PATH test_relative_path ${SFGE_TEST_DIR} ${test_file})
    string( REPLACE ".cpp" "" test_name ${test_relative_path} )
    add_executable(${test_name} ${SFGE_TEST_DIR}/${test_relative_path})
    # Make sure YourLib is linked to each app
    target_link_libraries(${test_name} PUBLIC SFGE_COMMON)
	set_property(TARGET ${test_name} PROPERTY CXX_STANDARD 17)
	IF(UNIX)
		set_target_properties(${test_name} PROPERTIES COMPILE_FLAGS "-save-temps")
	ENDIF(UNIX)
  IF(APPLE)
    set_target_properties(${test_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
  ENDIF(APPLE)
endforeach()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS}" )
elseif()
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -masm=intel" )
endif()
#SFGE
add_executable(SFGE src/main.cpp)

MESSAGE("SFGE_COMMON: " ${SFGE_COMMON})

target_link_libraries(SFGE PUBLIC SFGE_COMMON)
set_property(TARGET SFGE PROPERTY CXX_STANDARD 17)
if(APPLE)
  set_target_properties(SFGE PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
ENDIF(APPLE)
file(COPY data/ DESTINATION ${CMAKE_BINARY_DIR}/data/)
file(COPY scripts/ DESTINATION ${CMAKE_BINARY_DIR}/scripts/)
