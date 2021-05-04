cmake_minimum_required(VERSION 3.1.1)

set (CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME thief-fighters)
project (${PROJECT_NAME} LANGUAGES C CXX)

option(DATAPATH "Path to data Directory" ON)

#-DCMAKE_BUILD_TYPE=Release
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -Wall -DDATADIR=${DATAPATH} -fPIE")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -Wall -DDATADIR=${DATAPATH} -fPIE")

#SDL2 specific stuff and its add on libraries
INCLUDE(FindPkgConfig)

#pkg_check_modules(openal REQUIRED openal)

# Let CMake search for a raylib-config.cmake
find_package(raylib 3.5 QUIET) 

#Pugi XML parsing
find_path(pugixml_INCLUDE_DIRS pugixml.hpp)
find_library(pugixml_LIBRARIES NAMES pugixml)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

#For the shared library:
set ( PROJECT_LINK_LIBS  raylib ${SDL2_LIBRARIES} ${pugixml_LIBRARIES})
#where to find library files .so 
link_directories( /usr/lib /usr/local/lib ) 

set ( PROJECT_INCLUDE_DIR  ${SDL2_INCLUDE_DIRS})

#for where to find header files for source and library
include_directories(/usr/include /usr/local/include ./src ${PROJECT_INCLUDE_DIR})

if (MINGW)
    # need to link with mingw32 before SDL2
    list(APPEND _link_libraries mingw32)
endif ()

if(WIN32)
	set ( PROJECT_LINK_LIBS  ${PROJECT_LINK_LIBS} opengl32 gdi32 winmm)
endif()

set(SOURCES 
src/main.cpp
src/misc/StageManager.cpp
src/misc/globalvariables.cpp
src/misc/MediaLoader.cpp
src/misc/num_player_setter.cpp
src/misc/char_selector.cpp
src/misc/CharacterAssetManager.cpp
src/misc/stage_selector.cpp
src/systems/AttackPowerMechanicSystem.cpp
src/systems/PlayerDeathSystem.cpp
src/systems/PhysicsSystem.cpp
src/systems/CameraSystem.cpp
src/systems/AnimationSystem.cpp
src/systems/RenderSystem.cpp
src/systems/InputReactorSystem.cpp
src/systems/EnemySystem.cpp
src/core/ControllerInputHandler.cpp src/core/KeyboardTypingInputHandler.cpp
			 )

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

#make executable sphere from simple-sphere.cpp 
add_executable(${PROJECT_NAME} ${SOURCES})
#link libraries
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS})
