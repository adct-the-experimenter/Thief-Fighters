cmake_minimum_required(VERSION 3.1.1)

set (CMAKE_CXX_STANDARD 11)

set(PROJECT_NAME thief-fighters-world-editor)
project (${PROJECT_NAME} LANGUAGES C CXX)

option(DATAPATH "Path to data Directory" ON)

#emscripten specific cmake flags to add.
if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=2 -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2  -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()


#-DCMAKE_BUILD_TYPE=Release
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -Wall -DDATADIR=${DATAPATH} -fPIE")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Wall -DDATADIR=${DATAPATH} -fPIE")



#SDL2 specific stuff and its add on libraries
INCLUDE(FindPkgConfig)

#pkg_check_modules(openal REQUIRED openal)

# Let CMake search for a raylib-config.cmake
find_package(raylib 3.5 QUIET) 

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

if(EMSCRIPTEN)

set ( PROJECT_LINK_LIBS  raylib ${SDL2_LIBRARIES} libpugixml.a )


else()

#Pugi XML parsing
find_path(pugixml_INCLUDE_DIRS pugixml.hpp)
find_library(pugixml_LIBRARIES NAMES pugixml)
set ( PROJECT_LINK_LIBS  raylib ${SDL2_LIBRARIES} ${pugixml_LIBRARIES})

endif()



#For the shared library:


#where to find library files .so
if (DEFINED EMSCRIPTEN)

link_directories( ../web_compile_resources/lib)

else ()

link_directories( /usr/lib /usr/local/lib)

endif()
 
 

set ( PROJECT_INCLUDE_DIR  ${SDL2_INCLUDE_DIRS} ./src ../src)

#for where to find header files for source and library
if (EMSCRIPTEN)

include_directories(  ${PROJECT_INCLUDE_DIR} ../web_compile_resources/include)

else ()

include_directories(/usr/include /usr/local/include ${PROJECT_INCLUDE_DIR})

endif()


if (MINGW)
    # need to link with mingw32 before SDL2
    list(APPEND _link_libraries mingw32)
endif ()

if(WIN32)
	set ( PROJECT_LINK_LIBS  ${PROJECT_LINK_LIBS} opengl32 gdi32 winmm)
endif()



set( SOURCES 
src/main.cpp
src/world_editor.cpp
../src/misc/StageManager.cpp
../src/misc/globalvariables.cpp
../src/misc/MediaLoader.cpp
../src/misc/game_mode_selector.cpp ../src/misc/num_player_setter.cpp
../src/misc/char_selector.cpp
../src/misc/CharacterAssetManager.cpp
../src/misc/stage_selector.cpp
../src/systems/AttackPowerMechanicSystem.cpp
../src/systems/PlayerDeathSystem.cpp
../src/systems/PhysicsSystem.cpp
../src/systems/CameraSystem.cpp
../src/systems/AnimationSystem.cpp
../src/systems/RenderSystem.cpp
../src/systems/InputReactorSystem.cpp
../src/systems/EnemySystem.cpp
../src/core/ControllerInputHandler.cpp ../src/core/KeyboardTypingInputHandler.cpp
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

#make install ops
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
