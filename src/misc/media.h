#ifndef MEDIA_H
#define MEDIA_H

#include "raylib.h"
#include <array>
#include <string>

extern Texture2D title_menu_texture;

extern Texture2D char_select_texture;

extern Texture2D base_fighter_texture;


struct OneFrameGroup
{
	Rectangle frame;
};

//struct to hold array for 2 frames
struct TwoFrameGroup
{
	std::array <Rectangle,2> frames;
};

struct CharFrames
{
	//frames for staying still
	OneFrameGroup left_still_frame;
	OneFrameGroup right_still_frame;
	OneFrameGroup up_still_frame;
	OneFrameGroup down_still_frame;
	
	//frame for walking
	TwoFrameGroup right_walk_frames;
	TwoFrameGroup left_walk_frames;
	
	//frames for attack modes
	//organized like this to use animation attack mode as index of array for quicker processing
	//first is regular attack, 0
	// last frames are special attack/ability, 1-8
	std::array <TwoFrameGroup,9> right_attack_mode_frames;
	std::array <TwoFrameGroup,9> left_attack_mode_frames;
	
};


//list of characters avaiable for playing
extern std::array <std::string,8> character_names;

extern std::array <Texture2D,8> character_profile_textures;

extern std::array <Texture2D,8> character_sheet_textures;

extern std::array <CharFrames,8> character_frame_animations;

struct RequestedCharacters
{
	//array to hold the character number asked by 8 players
	//index is player who requested char. value is the name of char requested.
	// -1 is null request
	std::array <std::string,8> requested_by_player;
	
};

#endif
