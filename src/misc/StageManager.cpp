#include "StageManager.h"

#include "pugixml.hpp"
#include <iostream>

#include "globalvariables.h"
#include "raylib.h"
#include <fstream>

Stage main_stage;

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
			if(count >= 5){break;}
			
			crect++;
		}
		
		newfile.close();
	}
}

bool StageManager::LoadLevel(std::uint16_t level)
{
	//load map
	std::string collisionMapFilePath = "";
	std::string textureFilePath = "";
	
	//set file paths
	switch(level)
	{
		case 0:
		{  
			collisionMapFilePath = DATADIR_STR + "/stage_assets/training_stage/training_map_collisions.txt";
			textureFilePath = DATADIR_STR + "/stage_assets/training_stage/training_map_image.png";
			break;
		}
	}
	
	//load level based on file paths
	if(collisionMapFilePath != "" && textureFilePath != "")
	{
		
		main_stage.texture = LoadTexture(textureFilePath.c_str());    // Load map texture
		
		uint8_t max_num_rect = 5;
		ProcessCollisionMap(&main_stage.collision_rect_array[0],max_num_rect,collisionMapFilePath.c_str());
	}
	else
	{
		std::cout << "Uninitialized map file path or texture file paths.";
		return false;
	}
	
	return true;
}

void StageManager::FreeCurrentLoadedLevel()
{
	UnloadTexture(main_stage.texture);
	
}
