#include "CharacterAssetManager.h"

#include "globalvariables.h"
#include "pugixml.hpp"

//define character variables from media.h
std::array <std::string,8> character_names;
std::array <Texture2D,8> character_profile_textures;

bool CharacterAssetManager::LoadCharacterProfilesFromXML()
{
	//read name and file paths to character profile image from xml file with PugiXML
	
	std::string filepath = DATADIR_STR + "/fighter_assets/char-profiles-registry.xml";
	
	//for each requested chracter
	//read frame data by pugi xml
	// Create empty XML document within memory
    pugi::xml_document doc;
    
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(filepath.c_str(),
												pugi::parse_default);
    if (!result)
    {
		std::cout << "File: " << filepath << std::endl;
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
        return false;
    }
    
    pugi::xml_node root = doc.child("Root");
    
    //set up tile selector based on data
    pugi::xml_node profileRoot = root.child("Profiles");
    
    size_t iterator = 0;
	//go through each tile type in tiles node
	for (pugi::xml_node profile_node = profileRoot.first_child(); profile_node; profile_node = profile_node.next_sibling())
	{	
				
		std::string nameString = profile_node.attribute("name").value();
		character_names[iterator] = nameString;
		
		std::string filepath_texture = profile_node.attribute("img_path").value();
		
		std::string filePathTextureFull = DATADIR_STR + "/fighter_assets/" + filepath_texture;
		
		iterator++;
	}
	
	return true;
}

std::array <Texture2D,8> character_sheet_textures;

bool CharacterAssetManager::LoadCharacterAssets(RequestedCharacters& req_chars, std::uint8_t num_players)
{
	//read name and file paths to character profile image from xml file with PugiXML
	
	std::string filepath = DATADIR_STR + "/fighter_assets/char-registry.xml";
	
	//for each requested chracter
	//read frame data by pugi xml
	// Create empty XML document within memory
    pugi::xml_document doc;
    
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(filepath.c_str(),
												pugi::parse_default);
    if (!result)
    {
		std::cout << "File: " << filepath << std::endl;
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
        return false;
    }
    
    pugi::xml_node root = doc.child("Root");
    
    //set up tile selector based on data
    pugi::xml_node charRoot = root.child("Characters");
    
    std::array <std::string,8> filepaths_char_animations;
        
    //for each player
    for(std::uint8_t player_iterator = 0; player_iterator < num_players; player_iterator++)
    {
		//go through each character in character node
		for (pugi::xml_node char_node = charRoot.first_child(); char_node; char_node = char_node.next_sibling())
		{
			std::string nameString = char_node.attribute("name").value();
			
			if(nameString == req_chars.requested_by_player[player_iterator])		
			{
									
				std::string filepath_texture_sheet = char_node.attribute("sheet_path").value();
				
				std::string filePathTextureSheetFull = DATADIR_STR + "/fighter_assets/" + filepath_texture_sheet;
				
				character_sheet_textures[player_iterator] = LoadTexture(filePathTextureSheetFull.c_str());
				
				std::string filepath_frames = char_node.attribute("frame_path").value();
				filepaths_char_animations[player_iterator] = DATADIR_STR + "/fighter_assets/" + filepath_frames;
			}
			
		}
		
		if(filepaths_char_animations[player_iterator] == "")
		{
			std::cout << "Failed to load assets for requested character " << req_chars.requested_by_player[player_iterator] << std::endl;
			return false;
			break;
		}
	}
	
	//load the frame animation data from xml files

	
	return true;
}
