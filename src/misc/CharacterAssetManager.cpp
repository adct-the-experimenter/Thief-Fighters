#include "CharacterAssetManager.h"


#include "globalvariables.h"
#include "pugixml.hpp"

#include <vector>

#include "core/system.h"
#include "core/coordinator.h"

//define character variables from media.h
std::array <std::string,8> character_names;
std::array <Texture2D,8> character_profile_textures;
std::array <Texture2D,8> character_sheet_textures;
std::array <CharFrames,8> character_frame_animations;

extern Coordinator gCoordinator;

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

static void ReadReactangleFromXMLFrameSingle(Rectangle& rect, pugi::xml_node& frame_node)
{
	
	rect.x = atoi( std::string(frame_node.attribute("x").value() ).c_str());
	rect.y = atoi( std::string(frame_node.attribute("y").value() ).c_str());
	rect.width = atoi( std::string(frame_node.attribute("w").value() ).c_str());
	rect.height = atoi( std::string(frame_node.attribute("h").value() ).c_str());
	
}

static bool LoadFrameAnimationFromThisFile(std::string filepath_frames, CharFrames& char_frame)
{
	
	//for each requested chracter
	//read frame data by pugi xml
	// Create empty XML document within memory
    pugi::xml_document doc;
    
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(filepath_frames.c_str(),
												pugi::parse_default);
    if (!result)
    {
		std::cout << "File: " << filepath_frames << std::endl;
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
        return false;
    }
    
    pugi::xml_node root = doc.child("Root");
    
    //set up tile selector based on data
    pugi::xml_node framesRoot = root.child("Frames");
    
    std::array <Rectangle,36> frames;
    
    size_t iterator = 0;
	//go through each frame in frames node
	for (pugi::xml_node frame_node = framesRoot.first_child(); frame_node; frame_node = frame_node.next_sibling())
	{	
		ReadReactangleFromXMLFrameSingle(frames[iterator],frame_node);
		iterator++;
	}
	
	//get left still frame
		
	char_frame.left_still_frame.frame = frames[0];
	
	//get right still frame
	char_frame.right_still_frame.frame = frames[1];
	
	//get up still frame
	char_frame.up_still_frame.frame = frames[2];
	
	//get down still frame
	char_frame.down_still_frame.frame = frames[3];
	
	//get left walk frames
	char_frame.left_walk_frames.frames[0] = frames[4];
	char_frame.left_walk_frames.frames[1] = frames[5];
	
	//get right walk frames
	char_frame.right_walk_frames.frames[0] = frames[6];
	char_frame.right_walk_frames.frames[1] = frames[7];
	
	//get left attack frames
	// 0 is regular, 1-9 is special
	char_frame.left_attack_mode_frames[0].frames[0] = frames[8];
	char_frame.left_attack_mode_frames[0].frames[1] = frames[9];
	
	char_frame.left_attack_mode_frames[1].frames[0] = frames[10];
	char_frame.left_attack_mode_frames[1].frames[1] = frames[11];
	
	char_frame.left_attack_mode_frames[2].frames[0] = frames[12];
	char_frame.left_attack_mode_frames[2].frames[1] = frames[13];
	
	char_frame.left_attack_mode_frames[3].frames[0] = frames[14];
	char_frame.left_attack_mode_frames[3].frames[1] = frames[15];
	
	char_frame.left_attack_mode_frames[4].frames[0] = frames[16];
	char_frame.left_attack_mode_frames[4].frames[1] = frames[17];
	
	char_frame.left_attack_mode_frames[5].frames[0] = frames[18];
	char_frame.left_attack_mode_frames[5].frames[1] = frames[19];
	std::cout << "left attack mode big: " << char_frame.left_attack_mode_frames[5].frames[0].x << ", "
	<< char_frame.left_attack_mode_frames[5].frames[0].y << ", " << 
	char_frame.left_attack_mode_frames[5].frames[0].width << ", " << 
	char_frame.left_attack_mode_frames[5].frames[0].height << std::endl;
	
	//get right attack frames
	// 0 is regular attack, 1-9 is special
	char_frame.right_attack_mode_frames[0].frames[0] = frames[20];
	char_frame.right_attack_mode_frames[0].frames[1] = frames[21];
	
	char_frame.right_attack_mode_frames[1].frames[0] = frames[22];
	char_frame.right_attack_mode_frames[1].frames[1] = frames[23];
	
	char_frame.right_attack_mode_frames[2].frames[0] = frames[24];
	char_frame.right_attack_mode_frames[2].frames[1] = frames[25];
	
	char_frame.right_attack_mode_frames[3].frames[0] = frames[26];
	char_frame.right_attack_mode_frames[3].frames[1] = frames[27];
	
	char_frame.right_attack_mode_frames[4].frames[0] = frames[28];
	char_frame.right_attack_mode_frames[4].frames[1] = frames[29];
	
	char_frame.right_attack_mode_frames[5].frames[0] = frames[30];
	char_frame.right_attack_mode_frames[5].frames[1] = frames[31];
	
	//get up attack frames
	
	char_frame.up_attack_mode_frames[0].frames[0] = char_frame.up_still_frame.frame;
	char_frame.up_attack_mode_frames[0].frames[1] = char_frame.up_still_frame.frame;
	
	char_frame.up_attack_mode_frames[1].frames[0] = char_frame.right_attack_mode_frames[1].frames[0];
	char_frame.up_attack_mode_frames[1].frames[1] = char_frame.right_attack_mode_frames[1].frames[1];
	
	char_frame.up_attack_mode_frames[2].frames[0] = char_frame.right_attack_mode_frames[2].frames[0];
	char_frame.up_attack_mode_frames[2].frames[1] = char_frame.right_attack_mode_frames[2].frames[1];
	
	char_frame.up_attack_mode_frames[3].frames[0] = char_frame.right_attack_mode_frames[3].frames[0];
	char_frame.up_attack_mode_frames[3].frames[1] = char_frame.right_attack_mode_frames[3].frames[1];
	
	char_frame.up_attack_mode_frames[4].frames[0] = char_frame.right_attack_mode_frames[4].frames[0];
	char_frame.up_attack_mode_frames[4].frames[1] = char_frame.right_attack_mode_frames[4].frames[1];
	
	char_frame.up_attack_mode_frames[5].frames[0] = char_frame.right_attack_mode_frames[5].frames[0];
	char_frame.up_attack_mode_frames[5].frames[1] = char_frame.right_attack_mode_frames[5].frames[1];
	
	//get down attack frames
    
    char_frame.down_attack_mode_frames[0].frames[0] = char_frame.down_still_frame.frame;
	char_frame.down_attack_mode_frames[0].frames[1] = char_frame.down_still_frame.frame;
	
	char_frame.down_attack_mode_frames[1].frames[0] = char_frame.left_attack_mode_frames[1].frames[0];
	char_frame.down_attack_mode_frames[1].frames[1] = char_frame.left_attack_mode_frames[1].frames[1];
	
	char_frame.down_attack_mode_frames[2].frames[0] = char_frame.left_attack_mode_frames[2].frames[0];
	char_frame.down_attack_mode_frames[2].frames[1] = char_frame.left_attack_mode_frames[2].frames[1];
	
	char_frame.down_attack_mode_frames[3].frames[0] = char_frame.left_attack_mode_frames[3].frames[0];
	char_frame.down_attack_mode_frames[3].frames[1] = char_frame.left_attack_mode_frames[3].frames[1];
	
	char_frame.down_attack_mode_frames[4].frames[0] = char_frame.left_attack_mode_frames[4].frames[0];
	char_frame.down_attack_mode_frames[4].frames[1] = char_frame.left_attack_mode_frames[4].frames[1];
	
	char_frame.down_attack_mode_frames[5].frames[0] = char_frame.left_attack_mode_frames[5].frames[0];
	char_frame.down_attack_mode_frames[5].frames[1] = char_frame.left_attack_mode_frames[5].frames[1];
	
	//hurt frames
	
	char_frame.left_hurt_frames.frames[0] = frames[32];
	char_frame.left_hurt_frames.frames[1] = frames[33];
	
	char_frame.right_hurt_frames.frames[0] = frames[34];
	char_frame.right_hurt_frames.frames[1] = frames[35];
	
    return true;
}



static bool ReadCharacterStatsFromFile(std::string filepath_frames,CharStats& char_stat)
{
	//read frame data by pugi xml
	// Create empty XML document within memory
    pugi::xml_document doc;
    
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(filepath_frames.c_str(),
												pugi::parse_default);
    if (!result)
    {
		std::cout << "File: " << filepath_frames << std::endl;
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
        return false;
    }
    
    pugi::xml_node root = doc.child("Root");
    
    pugi::xml_node stats_node = root.child("Stats");
    
    
    char_stat.collision_box_width = atof( std::string( stats_node.attribute("c_box_width").value() ).c_str() ); 
    char_stat.collision_box_height =  atof( std::string( stats_node.attribute("c_box_height").value() ).c_str() ); 
    char_stat.attack_box_offset =  atof( std::string( stats_node.attribute("attackbox_offset").value() ).c_str() ); 
    char_stat.health_factor = atof( std::string( stats_node.attribute("health_factor").value() ).c_str() );
    char_stat.speed_factor = atof( std::string( stats_node.attribute("speed_factor").value() ).c_str() );
    char_stat.jump_factor = atof( std::string( stats_node.attribute("jump_factor").value() ).c_str() );
    char_stat.damage_factor = atof( std::string( stats_node.attribute("damage_factor").value() ).c_str() );
       
	return true;
}

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
    std::array <std::string,8> filepaths_char_stats;
    
    //initialize which characters to load
    std::vector <std::string> char_to_load_vec;
    std::vector <std::uint16_t> char_index_to_load_vec;
    
    std::array <bool,8> char_req_bool;
    
    for(size_t i = 0; i < num_players; i++)
    {
		//if character not already requested
		if(!char_req_bool[req_chars.char_texture_index_req[i]] )
		{
			//add it to vector
			char_to_load_vec.push_back( character_names[req_chars.char_texture_index_req[i]] );
			char_index_to_load_vec.push_back(req_chars.char_texture_index_req[i]);
			char_req_bool[req_chars.char_texture_index_req[i]] = true;
		}
		
	}
        
    //for each player
    for(std::uint8_t char_iterator = 0; char_iterator < char_to_load_vec.size(); char_iterator++)
    {
		//go through each character in character node
		for (pugi::xml_node char_node = charRoot.first_child(); char_node; char_node = char_node.next_sibling())
		{
			std::string nameString = char_node.attribute("name").value();
			
			if(nameString == char_to_load_vec[char_iterator])
			{
									
				std::string filepath_texture_sheet = char_node.attribute("sheet_path").value();
				
				std::string filePathTextureSheetFull = DATADIR_STR + "/fighter_assets/" + filepath_texture_sheet;
				
				character_sheet_textures[ char_index_to_load_vec[char_iterator] ] = LoadTexture(filePathTextureSheetFull.c_str());
				
				std::string filepath_frames = char_node.attribute("frame_path").value();
				filepaths_char_animations[ char_index_to_load_vec[char_iterator] ] = DATADIR_STR + "/fighter_assets/" + filepath_frames;
				
				std::string filepath_stats = char_node.attribute("stat_path").value();
				filepaths_char_stats[ char_index_to_load_vec[char_iterator] ] = DATADIR_STR + "/fighter_assets/" + filepath_stats;
			}
			
		}
		
		if(filepaths_char_animations[ char_index_to_load_vec[char_iterator] ] == "")
		{
			std::cout << "\nFailed to load assets for requested character " << character_names[char_iterator] << std::endl;
			return false;
			break;
		}
		
		if(filepaths_char_stats[ char_index_to_load_vec[char_iterator] ] == "")
		{
			std::cout << "\nFailed to load stats for requested character " << character_names[char_iterator] << std::endl;
			return false;
			break;
		}
	}
	
	//load the frame animation data from xml files
	for(std::uint8_t char_iterator = 0; char_iterator < char_to_load_vec.size(); char_iterator++)
	{
		if( !LoadFrameAnimationFromThisFile(filepaths_char_animations[ char_index_to_load_vec[char_iterator] ], character_frame_animations[ char_index_to_load_vec[char_iterator] ] ) )
		{
			std::cout << "\nFailed to load frame animations for requested character " << req_chars.requested_by_player[char_iterator] << std::endl;
			return false;
			break;
		}
	}
	
	//assuming first entities have player components
	//assign stats to player
	for(int i = 0; i < num_players; i++)
	{
		auto& collision_box = gCoordinator.GetComponent<CollisionBox>(i);
		auto& player = gCoordinator.GetComponent<Player>(i);
		
		CharStats stat;
		
		ReadCharacterStatsFromFile( filepaths_char_stats[ req_chars.char_texture_index_req[i] ], stat);
		
		
		collision_box.width = stat.collision_box_width;
		collision_box.height = stat.collision_box_height;
		
		
		player.attack_box_offset = stat.attack_box_offset;
		player.health_factor = stat.health_factor;
		player.speed_factor = stat.speed_factor;
		player.jump_factor = stat.jump_factor;
		player.damage_factor = stat.damage_factor;
		
		std::cout << "Player " << i << " , req char index " << req_chars.char_texture_index_req[i] << " initialized!" 
		<< " from file " << filepaths_char_stats[ req_chars.char_texture_index_req[i] ] << std::endl;
	}
	
	
	return true;
}



