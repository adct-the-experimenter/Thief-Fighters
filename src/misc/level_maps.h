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

//****************************************
//************** Stage *******************
//****************************************

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

//****************************************
//************** World *******************
//****************************************

enum class TileType : std::uint8_t {NONE=0,BACKGROUND,PUSH_BACK};

struct Tile
{
	float x;
	float y;
	TileType type;
	uint8_t tile_id;
	Rectangle* frame_clip_ptr;
};

struct World
{
	//name of the world
	std::string name;
	
	//holds entire 2d texture of tilesheet for world
	Texture2D tilesheet_texture;
	
	//tile dimensions
	std::uint8_t tileWidth;
	std::uint8_t tileHeight;
	
	//holds all tiles that make up the world
	//world is just a square for easier time calculating indices around player
	//30x30 tile size
	//300 tiles x 300 tiles
	std::vector <Tile> tiles_vector; 
	
	//holds positions for 8 players
	std::array <PlayerStart,8> player_start_array;
	
	//background texture scrolling
	//indicates if world uses scrolling background
	bool scrolling_bg = false;
	Texture2D scroll_bg_texture;
	float scrollSpeed = 0;
	
	//bool to indicate if world is in active use i.e. inhabited by a player
	bool in_active_use;
	
	//holds frames from tilesheet
	std::vector <Rectangle> frame_clip_map;
};

//The 4 worlds that players can inhabit simultaneously
//not actually world names

//by default world one is edited by world editor


extern World world_one;
extern World world_two;
extern World world_three;
extern World world_four;



#endif
