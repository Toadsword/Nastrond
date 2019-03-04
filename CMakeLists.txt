cmake_minimum_required(VERSION 3.10)
project(SFGE)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(IntelCompileFeatures)

include(EnumeratedOption)
include(CheckCXXFeature)
include(CheckCXXStandardLibrary)
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

if(WIN32)
add_compile_definitions(NOMINMAX)
endif(WIN32)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_VISIBILITY_PRESET hidden)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals)

include_directories(include ${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SFGE_AUDIO_SRC src/audio/*.cpp include/audio/*.h)
file(GLOB_RECURSE SFGE_ENGINE_SRC src/engine/*.cpp include/engine/*.h)
file(GLOB_RECURSE SFGE_EDITOR_SRC src/editor/*.cpp include/editor/*.h)
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
source_group("Editor"            FILES ${SFGE_EDITOR_SRC})
source_group("Graphics"            FILES ${SFGE_GRAPHICS_SRC})
source_group("Input"            FILES ${SFGE_INPUT_SRC})
source_group("Physics"            FILES ${SFGE_PHYSICS_SRC})
source_group("Python"            FILES ${SFGE_PYTHON_SRC})
source_group("Utility"            FILES ${SFGE_UTILITY_SRC})
List(APPEND SFGE_SRC  ${SFGE_AUDIO_SRC} ${SFGE_ENGINE_SRC} ${SFGE_EDITOR_SRC} ${SFGE_GRAPHICS_SRC}
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


#Google Test
set(GOOGLE_TEST_DIR ${EXTERNAL_DIR}/googletest)
add_subdirectory(${GOOGLE_TEST_DIR})
include_directories(${GOOGLE_TEST_DIR}/include)
set_target_properties (gtest gtest_main PROPERTIES
		FOLDER GTest)
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
set(PYBIND11_PYTHON_VERSION 3.6 CACHE STRING "Python version to use for compiling modules")
set(PYBIND11_DIR ${EXTERNAL_DIR}/pybind11/)
include_directories(${PYBIND11_DIR}/include)
IF(UNIX)
	add_definitions(-Wall -Wextra -pedantic)
	
	set(PYBIND11_CPP_STANDARD -std=c++1z)
	
	if(NOT APPLE)
		set(PYTHON_EXECUTABLE "/usr/bin/python3")
	elseif(APPLE)
		set(PYTHON_EXECUTABLE "/usr/local/bin/python3")
	endif()
	LIST(APPEND SFGE_LIBRARIES resolv)
	
	add_subdirectory(${PYBIND11_DIR})
	LIST(APPEND SFGE_LIBRARIES pybind11::embed)
ELSE()
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_subdirectory(${PYBIND11_DIR})
	LIST(APPEND SFGE_LIBRARIES pybind11::embed)
	#set(PYBIND11_CPP_STANDARD /std:c++latest)
	#find_package( PythonInterp 3.6 REQUIRED )
	#find_package( PythonLibs 3.6 REQUIRED )
	#include_directories(${PYTHON_INCLUDE_DIR}) 
	#LIST(APPEND SFGE_LIBRARIES ${PYTHON_LIBRARY})
ENDIF()


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
set(UTILITIES_DIR ${EXTERNAL_DIR}/utilities/)
include_directories(${UTILITIES_DIR})

#MISCELLANEOUS UTILITIES
if(UNIX AND NOT APPLE)
	check_include_file(uuid/uuid.h HAVE_UUID_UUID_H)
	check_library_exists(uuid uuid_generate "/lib;/usr/lib;/usr/local/lib" HAVE_UUID_LIB)

	if (HAVE_UUID_LIB)
		set(CMAKE_REQUIRED_LIBRARIES "uuid;${CMAKE_REQUIRED_LIBRARIES}")
	endif (HAVE_UUID_LIB)
endif()


set(REMOTERY_DIR ${EXTERNAL_DIR}/Remotery)
add_subdirectory(${REMOTERY_DIR})
include_directories(${REMOTERY_DIR})
LIST(APPEND SFGE_LIBRARIES Remotery)

#SFGE_COMMON

LIST(APPEND SFGE_LIBRARIES)
if(UNIX AND NOT APPLE)
	LIST(APPEND SFGE_LIBRARIES
		stdc++fs)
ENDIF()

include_directories(include/)
include_directories(extensions/include/)

file(GLOB_RECURSE SFGE_EXTENSIONS_SRC extensions/src/*.cpp extensions/include/*.h)

macro(GroupSources curdir)
	file(GLOB children RELATIVE ${CMAKE_SOURCE_DIR}/${curdir} ${CMAKE_SOURCE_DIR}/${curdir}/*)
	foreach(child ${children})
			if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				string(REPLACE "include\\extensions" "" groupname ${groupname})
				string(REPLACE "src" "" groupname ${groupname})
				string(REPLACE "extensions" "Extension" groupname ${groupname})
				source_group(${groupname} FILES ${CMAKE_SOURCE_DIR}/${curdir}/${child})
			endif()
	endforeach()
endmacro()

# Execute the macro
GroupSources(extensions)
add_library(SFGE_COMMON STATIC ${SFGE_SRC} ${SFGE_EXTENSIONS_SRC})


message("SFGE LIBRARIES : ${SFGE_LIBRARIES}")
target_link_libraries(SFGE_COMMON PUBLIC ${SFGE_LIBRARIES})
set_property(TARGET SFGE_COMMON PROPERTY CXX_STANDARD 17)

if(APPLE)
	set_target_properties(SFGE_COMMON PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
ENDIF()

#SFGE COURSES
SET(SFGE_COURSE_DIR ${CMAKE_SOURCE_DIR}/courses)
file(GLOB COURSE_FILES ${SFGE_COURSE_DIR}/*.cpp )
foreach(course_file ${COURSE_FILES} )
    # I used a simple string replace, to cut off .cpp.
    file(RELATIVE_PATH course_relative_path ${SFGE_COURSE_DIR} ${course_file})
    string( REPLACE ".cpp" "" course_name ${course_relative_path} )
    add_executable(${course_name} ${SFGE_COURSE_DIR}/${course_relative_path})
    # Make sure YourLib is linked to each app
    target_link_libraries(${course_name} PUBLIC SFGE_COMMON)
	set_property(TARGET ${course_name} PROPERTY CXX_STANDARD 17)
	IF(UNIX)
		set_target_properties(${course_name} PROPERTIES COMPILE_FLAGS "-save-temps")
	ENDIF(UNIX)
	set_target_properties (${course_name} PROPERTIES
    FOLDER Courses)
  IF(APPLE)
    set_target_properties(${course_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
  ENDIF()
endforeach()

#SFGE TEST
SET(SFGE_TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
file(GLOB TEST_FILES ${SFGE_TEST_DIR}/*.cpp )
add_executable(SFGE_TEST ${TEST_FILES} )
target_link_libraries(SFGE_TEST gtest gtest_main SFGE_COMMON)
if(APPLE )
	set_target_properties(SFGE_TEST PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
		ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
ENDIF()
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fvisibility=hidden" )
elseif()
	SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fvisibility=hidden -masm=intel" )
endif()
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

#SFGE
add_executable(SFGE src/main.cpp)

target_link_libraries(SFGE PUBLIC SFGE_COMMON)
set_property(TARGET SFGE PROPERTY CXX_STANDARD 17)
if(APPLE)
	set_target_properties(SFGE PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
ENDIF()

#SFGE TOOLS
SET(SFGE_TOOLS_DIR ${CMAKE_SOURCE_DIR}/tools)
file(GLOB TOOLS_DIR ${SFGE_TOOLS_DIR}/*)

foreach(tool_dir ${TOOLS_DIR})

    get_filename_component(tool_name ${tool_dir} NAME)
	LIST(APPEND TOOL_NAME_LIST ${tool_name})
	message("Tool name: ${tool_name} ${tool_dir}")
	include_directories(${tool_dir}/include/)
	file(GLOB_RECURSE TOOL_SRC ${tool_dir}/include/*.h ${tool_dir}/src/*.cpp)
	add_library(${tool_name}_library STATIC ${TOOL_SRC})
	LIST(APPEND TOOL_LIBRARIES ${tool_name}_library)
	target_link_libraries(${tool_name}_library PUBLIC SFGE_COMMON)
	set_property(TARGET ${tool_name}_library PROPERTY CXX_STANDARD 17)
	set_target_properties (${tool_name}_library PROPERTIES FOLDER Tools)
	if(APPLE)
		set_target_properties(${tool_name}_library PROPERTIES
				RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
				RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
	ENDIF()
	LIST(APPEND tools "\"${tool_name}\"")
endforeach()

execute_process(
		COMMAND ${PYTHON_EXECUTABLE} "scripts/tools/generate_tool_bindings.py" ${TOOL_NAME_LIST}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
		RESULT_VARIABLE py_result
)
target_link_libraries(SFGE_COMMON PUBLIC ${TOOL_LIBRARIES})
target_sources(SFGE PUBLIC ${CMAKE_SOURCE_DIR}/src/tools/tools_pch.cpp ${CMAKE_SOURCE_DIR}/include/tools/tools_pch.h)
target_sources(SFGE_TEST PUBLIC ${CMAKE_SOURCE_DIR}/src/tools/tools_pch.cpp ${CMAKE_SOURCE_DIR}/include/tools/tools_pch.h)

#copy folder to build
file(COPY data/ DESTINATION ${CMAKE_BINARY_DIR}/data/)
file(COPY scripts/ DESTINATION ${CMAKE_BINARY_DIR}/scripts/)


#write tool config file
string(REPLACE "\"\"" "\",\"" tools_list ${tools})
file (WRITE "${CMAKE_BINARY_DIR}/data/tools_config.json" "{\"tools\" : [${tools_list}], \"src_dir\": \"${CMAKE_SOURCE_DIR}\", \"bin_dir\": \"${CMAKE_BINARY_DIR}\"}")