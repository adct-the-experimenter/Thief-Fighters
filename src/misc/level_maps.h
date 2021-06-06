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

struct PlayerStart
{
	uint32_t player_num = 0;
	uint32_t x = 0;
	uint32_t y = 0;
};

struct Stage
{
	//holds entire 2d texture of stage
	Texture2D texture;
	//holds collision rectangles in stage, 5 max
	std::array <CollisionRect,10> collision_rect_array;
	//holds positions for 8 players
	std::array <PlayerStart,8> player_start_array;
	
	//background texture scrolling
	//indicates if stage uses scrolling background
	bool scrolling_bg = false;
	Texture2D scroll_bg_texture;
	float scrollSpeed = 0;
};

//only load and free 1 stage since only 1 stage is played at a time
extern Stage main_stage;

extern std::array <Texture2D,8> stage_profile_textures;

#endif
