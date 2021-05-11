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
	std::array <TwoFrameGroup,9> up_attack_mode_frames;
	std::array <TwoFrameGroup,9> down_attack_mode_frames;
	
	//frames for being hurt
	TwoFrameGroup right_hurt_frames;
	TwoFrameGroup left_hurt_frames;
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
	
	//array to hold character texture index for character asked by 8 players
	//index is the player who request character. value is the char texture index.
	std::array <std::int16_t,8> char_texture_index_req;
	
};

#endif
