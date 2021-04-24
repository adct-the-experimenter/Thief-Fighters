#ifndef LEVEL_MAP_H
#define LEVEL_MAP_H

#include "raylib.h"
#include <vector>
#include <cstdint>
#include <iostream>
#include <array>


struct CollisionRect
{
	bool initialized = false;
	Rectangle rect;
};

struct Stage
{
	//holds entire 2d texture of stage
	Texture2D texture;
	//holds collision rectangles in stage, 5 max
	std::array <CollisionRect,5> collision_rect_array;
};

//only load and free 1 stage since only 1 stage is played at a time
extern Stage main_stage;

#endif
