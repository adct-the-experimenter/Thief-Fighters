#include "StageManager.h"

#include "core/system.h"

#include "core/coordinator.h"
#include <iostream>

#include "globalvariables.h"
#include "raylib.h"
#include <fstream>

Stage main_stage;

extern Coordinator gCoordinator;

StageManager::StageManager()
{
	
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

bool StageManager::LoadLevel(std::uint16_t level)
{
	//load map
	std::string collisionMapFilePath = "";
	std::string textureFilePath = "";
	std::string playerStartFilePath = "";
	
	//set file paths
	switch(level)
	{
		case 0:
		{  
			collisionMapFilePath = DATADIR_STR + "/stage_assets/training_stage/training_map_collisions.txt";
			textureFilePath = DATADIR_STR + "/stage_assets/training_stage/training_map_image.png";
			playerStartFilePath = DATADIR_STR + "/stage_assets/training_stage/training_map_start.txt";
			break;
		}
	}
	
	//load level based on file paths
	if(collisionMapFilePath != "" && textureFilePath != "" && playerStartFilePath != "")
	{
		main_stage.texture = LoadTexture(textureFilePath.c_str());    // Load map texture
		
		uint8_t max_num_rect = 5;
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
		
		transform.position.x = main_stage.player_start_array[i].x - collision_box.width/2;
		transform.position.y = main_stage.player_start_array[i].y - collision_box.height;
		
	}
}
