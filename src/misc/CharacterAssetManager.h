#ifndef CHARACTER_ASSET_MANAGER
#define CHARACTER_ASSET_MANAGER

#include "raylib.h"
#include <array>

#include "misc/media.h"

class CharacterAssetManager
{
public:
	
	//function to load names of characters in character registry xml file
	bool LoadCharacterProfilesFromXML();
	
	//loads Character textures and sprite animations for chracters requested
	bool LoadCharacterAssets(RequestedCharacters& req_chars, std::uint8_t num_players);
	
	
};

#endif
