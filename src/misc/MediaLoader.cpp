#include "MediaLoader.h"

#include "globalvariables.h"
#include "char_media.h"

Texture2D title_menu_texture;
Texture2D char_select_texture;
Texture2D tutorial_texture;

MediaLoader::MediaLoader()
{
	
}

bool MediaLoader::loadMedia()
{
	std::string filepath = DATADIR_STR + "/title_menu.png";
	title_menu_texture = LoadTexture(filepath.c_str());
	
	filepath = DATADIR_STR + "/char_select.png";
	char_select_texture = LoadTexture(filepath.c_str());
	
	filepath = DATADIR_STR + "/controls-tutorial.png";
	tutorial_texture = LoadTexture(filepath.c_str());
	
	return true;
}

void MediaLoader::freeMedia()
{
	UnloadTexture(title_menu_texture);
	UnloadTexture(char_select_texture);
	UnloadTexture(tutorial_texture);
	
	/*
	for(size_t i = 0; i < character_sheet_textures.size(); i++)
	{
		UnloadTexture(character_sheet_textures[i]);
	}
	
	for(size_t i = 0; i < character_profile_textures.size(); i++)
	{
		UnloadTexture(character_profile_textures[i]);
	}
	*/
}
