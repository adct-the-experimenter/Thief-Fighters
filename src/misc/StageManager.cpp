#include "StageManager.h"

#include "core/system.h"

#include "core/coordinator.h"
#include <iostream>

#include "globalvariables.h"
#include "raylib.h"
#include <fstream>

#include "pugixml.hpp"

Stage main_stage;

extern Coordinator gCoordinator;

std::array <Texture2D,8> stage_profile_textures;

StageManager::StageManager()
{
	max_num_rect = 10;
	stages_info_loaded = false;
	
	num_stages_initialized = 0;
}



static void ProcessCollisionMap(CollisionRect* crect,uint8_t max_num_rect,const char* filepath)
{
	std::ifstream newfile;
	newfile.open(filepath,std::ios::in); //open a file to perform read operation using file object
	if (newfile.is_open())
	{   
		
		int x, y, width, height;
		
		uint8_t count = 0;
		
		//read file line by line for rectangle parameters
		while (newfile >> x >> y >> width >> height)
		{
			// process rectangle parameters
			crect->rect.x = x;
			crect->rect.y = y;
			crect->rect.width = width;
			crect->rect.height = height;
			crect->initialized = true; 
			
			count++;
			
			//break loop if exceed max number
			if(count >= max_num_rect){break;}
			
			crect++;
		}
		
		newfile.close();
	}
}

static void ProcessPlayerStartInfo(PlayerStart* pstart, uint8_t max_players, const char* filepath)
{
	std::ifstream newfile;
	newfile.open(filepath,std::ios::in); //open a file to perform read operation using file object
	if (newfile.is_open())
	{   
		
		int x, y;
		
		uint8_t count = 0;
		
		//read file line by line for rectangle parameters
		while (newfile >> x >> y )
		{
			// process rectangle parameters
			pstart->x = x;
			pstart->y = y;
			pstart->player_num = count + 1; 
			
			count++;
			
			//break loop if exceed max number
			if(count >= max_players){break;}
			
			pstart++;
		}
		
		newfile.close();
	}
	else
	{
		std::cout << "Failed to open file in player start info!\n";
	}
}

bool StageManager::LoadLevel(int level)
{
	//load map
	std::string collisionMapFilePath = "";
	std::string textureFilePath = "";
	std::string playerStartFilePath = "";
	
	
	collisionMapFilePath = stages_info[level].collisionMapFilePath;
	textureFilePath = stages_info[level].textureFilePath;
	playerStartFilePath = stages_info[level].playerStartFilePath;
	
	
	//load level based on file paths
	if(collisionMapFilePath != "" && textureFilePath != "" && playerStartFilePath != "")
	{
		main_stage.texture = LoadTexture(textureFilePath.c_str());    // Load map texture
		
		//if it is a scrolling level
		if(stages_info[level].scrolling)
		{
			main_stage.scrolling_bg = true;
			main_stage.scroll_bg_texture = LoadTexture(stages_info[level].scrollingBGTextureFilePath.c_str());
			main_stage.scrollSpeed = stages_info[level].scrollSpeed;
		}
		
		ProcessCollisionMap(&main_stage.collision_rect_array[0],max_num_rect,collisionMapFilePath.c_str());
		
		uint8_t max_num_players = 8;
		ProcessPlayerStartInfo(&main_stage.player_start_array[0],max_num_players,playerStartFilePath.c_str());
	}
	else
	{
		std::cout << "Uninitialized map file path or texture file paths or player start file path.";
		return false;
	}
	
	return true;
}

void StageManager::FreeCurrentLoadedLevel()
{
	UnloadTexture(main_stage.texture);
	
}

void StageManager::PlacePlayersInStage(std::int8_t num_players)
{
	//assuming first entities have player components
	for(int i = 0; i < num_players; i++)
	{
		auto& transform = gCoordinator.GetComponent<Transform2D>(i);
		auto& collision_box = gCoordinator.GetComponent<CollisionBox>(i);
		
		transform.position.x = float(main_stage.player_start_array[i].x) - collision_box.width/2;
		transform.position.y = float(main_stage.player_start_array[i].y) - collision_box.height;
		
	}
}

bool StageManager::LoadStageProfiles()
{
	//read name and file paths to character profile image from xml file with PugiXML
	
	std::string filepath = DATADIR_STR + "/stage_assets/stage-profiles-registry.xml";
	
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
		stages_info[iterator].name = nameString;
		
		std::string filepath_texture = profile_node.attribute("img_path").value();
		
		std::string filePathTextureFull = DATADIR_STR + "/stage_assets/" + filepath_texture;
		stages_info[iterator].profileImageFilePath = filePathTextureFull;
		
		stage_profile_textures[iterator] = LoadTexture(stages_info[iterator].profileImageFilePath.c_str());
		
		iterator++;
	}
	
	return true;
}

bool StageManager::LoadStageGamePlayInfo()
{
	//read name and file paths to character profile image from xml file with PugiXML
	
	std::string filepath = DATADIR_STR + "/stage_assets/stage-registry.xml";
	
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
    pugi::xml_node stageRoot = root.child("Stages");
    
    size_t iterator = 0;
    
	//go through each character in character node
	for (pugi::xml_node stage_node = stageRoot.first_child(); stage_node; stage_node = stage_node.next_sibling())
	{
		
		std::string nameString = stage_node.attribute("name").value();
		
		//load filepath to collisions text
		std::string filepath_collisions = stage_node.attribute("collision_path").value();
			
		std::string filePathCollisionsFull = DATADIR_STR + "/stage_assets/" + filepath_collisions;
		
		stages_info[iterator].collisionMapFilePath = filePathCollisionsFull;
		
		//load filepath to start positions text
		std::string filepath_starts = stage_node.attribute("start_path").value();
			
		std::string filePathStartsFull = DATADIR_STR + "/stage_assets/" + filepath_starts;
		
		stages_info[iterator].playerStartFilePath = filePathStartsFull;
		
		//load filepath to texture
		std::string filepath_texture_sheet = stage_node.attribute("img_path").value();
			
		std::string filePathTextureFull = DATADIR_STR + "/stage_assets/" + filepath_texture_sheet;
		
		stages_info[iterator].textureFilePath = filePathTextureFull;
		
		//determine if stage has scrolling background, initialize other variables if so
		std::string scrolling_status = stage_node.attribute("scrolling").value();
		
		if(scrolling_status == "yes")
		{
			stages_info[iterator].scrolling = true;
			
			//load filepath to background texture
			std::string filepath_bg_texture = stage_node.attribute("scroll_bg_path").value();
			
			std::string filePathTextureBGFull = DATADIR_STR + "/stage_assets/" + filepath_bg_texture;
			
			stages_info[iterator].scrollingBGTextureFilePath = filePathTextureBGFull;
			
			//get scroll speed
			std::string scroll_speed_str = stage_node.attribute("scroll_speed").value();
			stages_info[iterator].scrollSpeed = atof(scroll_speed_str.c_str());
		}
		else
		{
			stages_info[iterator].scrolling = false;
		}
		
		
				
		stages_info[iterator].initialized = true;
		num_stages_initialized++;
		
		iterator++;
	}
		
	return true;
}

bool StageManager::LoadStageInfoFromXML()
{
	if(!StageManager::LoadStageProfiles())
	{
		std::cout << "Failed to load stage profiles!\n";
		return false;
	}
	
	if(!StageManager::LoadStageGamePlayInfo())
	{
		std::cout << "Failed to load stage game play info!\n";
		return false;
	}
	
	stages_info_loaded = true;
	
	return true;
}

bool StageManager::GetStagesInfoLoadedBool(){return stages_info_loaded;}

int StageManager::GetNumberOfStagesInitialized(){return num_stages_initialized;}
