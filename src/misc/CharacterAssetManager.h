#ifndef CHARACTER_ASSET_MANAGER
#define CHARACTER_ASSET_MANAGER

#include "raylib.h"
#include <array>

#include "misc/char_media.h"

class CharacterAssetManager
{
public:
	
	//function to load names of characters in character registry xml file
	bool LoadCharacterProfilesFromXML();
	
	//function to free character profiles that are currently loaded
	void FreeLoadedCharacterProfiles();
	
	//loads Character textures and sprite animations, sounds for chracters requested
	bool LoadCharacterAssets(RequestedCharacters& req_chars, std::uint8_t num_players);
	
	//unloads character	textures and sounds for characters requested
	void FreeCurrentlyLoadedCharacterAssets();
	
};

#endif
