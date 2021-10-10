#include "WorldSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>
#include <cmath>


#include "misc/globalvariables.h"

#include "pugixml.hpp"

extern Coordinator gCoordinator;

Room room_one;
Room room_two;
Room room_three;
Room room_four;

bool WorldSystem::Init()
{	
	//load file paths
	if( !WorldSystem::LoadWorldFilepaths() )
	{
		std::cout << "\nFailed to load tilesheet and level filepaths in world-registry.xml!\n";
		return false;
	}
	
	//load main world by default
	//change later when saving and loading from file is implemented
	
	if(!WorldSystem::LoadWorldLevel(&room_one,0))
	{
		std::cout << "\nFailed to load world level in world system initialization.\n";
		return false;
	}
	
	
	//place all players in main world by default
	//change later when saving and loading from file is implemented
	
	for (auto const& entity : mEntities)
	{
		auto& player = gCoordinator.GetComponent<Player>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
		
		player.world_id = 0;
		collisionBox.world_id = 0;
		
		//first 4 players are camera leads
		if(player.player_num < 5)
		{
			player.camera_lead = true;
			player.camera_num_leader = player.player_num - 1;
		}
		
		
	}
	
	//set all cameras to main world by default
	//change later when saving and loading from file is implemented
	for(size_t i = 0; i < m_camera_manager_ptr->lead_cameras.size(); i++)
	{
		m_camera_manager_ptr->lead_cameras[i].SetWorldID(0);
	}
	
	return true;
}

void WorldSystem::FreeResources()
{

	if(room_one.in_active_use)
	{
		FreeWorldLevel(&room_one);
	}
	
	if(room_two.in_active_use)
	{
		FreeWorldLevel(&room_one);
	}
	
	if(room_three.in_active_use)
	{
		FreeWorldLevel(&room_one);
	}
	
	if(room_four.in_active_use)
	{
		FreeWorldLevel(&room_one);
	}
}

void WorldSystem::SetPointerToCameraManager(CameraManager* cam_manager_ptr){m_camera_manager_ptr = cam_manager_ptr;}

bool WorldSystem::LoadWorldFilepaths()
{
	//read name and file paths to character profile image from xml file with PugiXML
	
	m_levels_filepaths_xml = DATADIR_STR + "/world_assets/world-registry.xml";
	
	//for each requested chracter
	//read frame data by pugi xml
	// Create empty XML document within memory
    pugi::xml_document doc;
    
    // Load XML file into memory
    // Remark: to fully read declaration entries you have to specify
    // "pugi::parse_declaration"
    pugi::xml_parse_result result = doc.load_file(m_levels_filepaths_xml.c_str(),
												pugi::parse_default);
    if (!result)
    {
		std::cout << "File: " << m_levels_filepaths_xml << std::endl;
        std::cout << "Parse error: " << result.description()
            << ", character pos= " << result.offset;
        return false;
    }
    
    pugi::xml_node root = doc.child("Root");
    
    pugi::xml_node worldRoot = root.child("Worlds");
    
    size_t iterator = 0;
    
	//go through each character in character node
	for (pugi::xml_node world_node = worldRoot.first_child(); world_node; world_node = world_node.next_sibling())
	{
		
		std::string nameString = world_node.attribute("name").value();
		
		//load filepath to tile sheet description xml
		std::string filepath_tsd = world_node.attribute("tsd_path").value();
			
		std::string filePathTSDFull = DATADIR_STR + "/world_assets/" + filepath_tsd;
		
		m_tilesheet_filepaths[iterator] = filePathTSDFull;
		
		//load filepath to level xml
		std::string filepath_level = world_node.attribute("level_path").value();
			
		std::string filePathLevelFull = DATADIR_STR + "/world_assets/" + filepath_level;
		
		m_world_levels_filepaths[iterator] = filePathLevelFull;
		
		iterator++;
		
		//stop loop if more than 4 worlds
		if(iterator >= 4){break;}
	}
		
	return true;
}

bool WorldSystem::LoadDataBasedOnTilesheetDescription(Room* world_ptr,std::string filepath)
{
	//read xml file 
		
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
    
    pugi::xml_node root = doc.child("TileDescriptionXML");
    
    //save path to tilesheet
    pugi::xml_node tilesheet_node = root.child("Tilesheet");
    std::string tilesheet_path = tilesheet_node.attribute("path").value();
    
    std::string tilesheetPathFull = DATADIR_STR + "/world_assets/" + tilesheet_path;
    
    //load tile sheet
    world_ptr->tilesheet_texture = LoadTexture(tilesheetPathFull.c_str());
        
    //set up tile selector based on data
    pugi::xml_node tileRoot = root.child("Tiles");
    
    size_t iterator = 0;
	//go through each tile type in tiles node
	for (pugi::xml_node tile_node = tileRoot.first_child(); tile_node; tile_node = tile_node.next_sibling())
	{	
		
		//set type
		TileType type = TileType::NONE;
		
		std::string valString = tile_node.attribute("type").value();
		//std::cout << valString << std::endl;
		
		if(valString ==  "PUSHBACK"){type = TileType::PUSH_BACK; }
		else if(valString ==  "BACKGROUND"){ type = TileType::BACKGROUND; }
		else{std::cout << "Tile type not handled!" << "i:" << iterator << std::endl; }
				
		//set frame clip
		
		std::uint32_t x,y,width,height;
		
		valString = tile_node.attribute("x").value();
		x = atoi(valString.c_str());
		
		valString = tile_node.attribute("y").value();
		y = atoi(valString.c_str());
				
		//set frame clip
		Rectangle frame_clip = {static_cast<float>(x),static_cast<float>(y),30,30};
		
		//set tile id
		valString = tile_node.attribute("id").value();
		std::uint16_t tile_id = atoi(valString.c_str());
		
		//push into frame clip map
		//assuming tile id is in squential order from least to greatest 0 - n
		world_ptr->frame_clip_map.push_back(frame_clip);
		
	}
	
	return true;	
}

bool WorldSystem::LoadDataFromXMLFile(Room* world_ptr,std::string mapFilePath, std::string tilesheetDescriptionFilePath)
{
	//read tilesheet description xml file for tile parameters
	if(!WorldSystem::LoadDataBasedOnTilesheetDescription(world_ptr,tilesheetDescriptionFilePath))
	{
		std::cout << "\nFailed to read tilesheet description xml!\n";
		return false;
	}
	else
	{
		// Create empty XML document within memory
		pugi::xml_document doc;
		
		// Load XML file into memory
		// Remark: to fully read declaration entries you have to specify
		// "pugi::parse_declaration"
		pugi::xml_parse_result result = doc.load_file(mapFilePath.c_str(),
													pugi::parse_default);
		if (!result)
		{
			std::cout << "File: " << mapFilePath << std::endl;
			std::cout << "Parse error: " << result.description()
				<< ", character pos= " << result.offset;
			return false;
		}
		
		pugi::xml_node root = doc.child("RootMap");
		
		//read data from level map xml file
		std::string valString;
				
		//for every tile in xml file
		
		pugi::xml_node tileRoot = root.child("Tiles");
		size_t iterator = 0;
		
		//go through each tile in tiles node
		for (pugi::xml_node tile_node = tileRoot.first_child(); tile_node; tile_node = tile_node.next_sibling())
		{	
			Tile tile;
			
			//set type
			TileType type = TileType::NONE;
			
			valString = tile_node.attribute("type").value();
			//std::cout << valString << std::endl;
			
			if(valString ==  "PUSHBACK"){type = TileType::PUSH_BACK; }
			else if(valString ==  "BACKGROUND"){ type = TileType::BACKGROUND; }
			else{std::cout << "Tile type not handled!" << "i:" << iterator << std::endl; }
			
			tile.type = type;
			
			//set tile number
			
			valString = tile_node.attribute("id").value();
			tile.tile_id = atoi(valString.c_str());
			
			//set frame clip pointer based on tile id
			
			tile.frame_clip_ptr = &world_ptr->frame_clip_map[tile.tile_id];
			
			//assign to vector
			world_ptr->tiles_vector[iterator] = tile;
			
			iterator++;
		}
		
		
		std::uint32_t x_offset = 0;
		std::uint32_t y_offset = 0;
		
		//number of horizontal tiles
		//10 sections * 640 pixels/section / 30 pixels/tile ~= 220
		size_t num_tiles_horiz = 220;
		size_t tile_height = 30;
		size_t tile_width = 30;
		
		//initialize tile position
		for(size_t i = 0; i < world_ptr->tiles_vector.size(); i++)
		{
			if(i % num_tiles_horiz == 0 && i != 0)
			{
				x_offset = 0;
				y_offset += tile_height;
			}
			
			world_ptr->tiles_vector[i].x = x_offset;
			world_ptr->tiles_vector[i].y = y_offset;
			
			x_offset += tile_width;
		}
		
	}
	
	return true;
    
}

bool WorldSystem::LoadWorldLevel(Room* world_ptr, std::uint8_t level_num)
{
	size_t num_tiles_horiz = 220;
	size_t square_area = num_tiles_horiz * num_tiles_horiz;
	
	world_ptr->tiles_vector.resize(square_area); 
	world_ptr->in_active_use = true;
	
	
	//load the xml file containing info on tile level positions and tile types
	
	
	//read xml file containing tile sheet path and tile positions
	//assuming tile is uniform size of 30x30 pixels
	//load texture for tile sheet
	//load the tile sheet clips
	
	std::string level_file_xml = m_world_levels_filepaths[level_num];
	std::string tilesheet_file_xml = m_tilesheet_filepaths[level_num];
	
	if(!WorldSystem::LoadDataFromXMLFile(world_ptr,level_file_xml, tilesheet_file_xml))
	{
		std::cout << "\nFailed to load data from xml files!\n";
		return false;
	}
	
	return true;
}

void WorldSystem::FreeWorldLevel(Room* world_ptr)
{		
	world_ptr->tiles_vector.clear();
	world_ptr->frame_clip_map.clear(); 
	world_ptr->in_active_use = false;
	UnloadTexture(world_ptr->tilesheet_texture);
	
}

void WorldSystem::logic(float& dt)
{
	//for all players
	for (auto const& entity : mEntities)
	{
		auto& player = gCoordinator.GetComponent<Player>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
			
		//get world that player is in
		//get player location in world
		
		//if player collides with transportation
			//handle transportation of players
			
			//prevent non-camera lead player from inhabiting world alone
			
			//if world is not already in active use, load the tiles for it
			
			//change world that player is in
			//change player location in world
			
			//set which camera player is in
	}
			
}

static void RenderLevelMapRelativeToCameraAndScreen(Room* world_ptr,Rectangle& camera,Rectangle& screen)
{
	
	//number of tiles in a row
	size_t num_tiles_horizontal = 220;
	
	//render 12 rows of tiles
	
	size_t start_tiles[12];
	size_t end_tiles[12];
	
	//get index based on top left corner of camera
	size_t horiz_index = trunc(camera.x / 30 );
	size_t vert_index = trunc( camera.y / 30 ) * num_tiles_horizontal;
	
	size_t rows_to_render = trunc( camera.height / 30);
	
	//initialize start tiles
	for(size_t i = 0; i < rows_to_render; i++)
	{
		start_tiles[i] = horiz_index + vert_index + i*num_tiles_horizontal;
		if(start_tiles[i] > world_ptr->tiles_vector.size()){start_tiles[i] = world_ptr->tiles_vector.size() - 1;}
	}
	
	//get camera width offset
	//added 2 tiles to prevent tearing when player is walking
	size_t camera_offset_width = trunc( camera.width / 30 ) + 2;
	
	//initialize end tiles
	for(size_t i = 0; i < rows_to_render; i++)
	{
		end_tiles[i] = start_tiles[i] + camera_offset_width;
		if(end_tiles[i] >= room_one.tiles_vector.size()){end_tiles[i] = room_one.tiles_vector.size() - 1;}
	}
	
	
	for(size_t i = 0; i < rows_to_render; i++)
	{
		for(size_t tile_index = start_tiles[i]; tile_index < end_tiles[i]; tile_index++)
		{
			
			Vector2 pos = {world_ptr->tiles_vector.at(tile_index).x - camera.x + screen.x,world_ptr->tiles_vector.at(tile_index).y - camera.y + screen.y};
			
			//std::cout << "pos : " << pos.x << " , " << pos.y << std::endl;
			if(world_ptr->tiles_vector.at(tile_index).frame_clip_ptr)
			{
				DrawTextureRec(world_ptr->tilesheet_texture, 
						   *world_ptr->tiles_vector.at(tile_index).frame_clip_ptr, 
						   pos, 
						   WHITE);
			}
		}
		
		
	}
	
}

//#define DEBUG_CAMERA_SYSTEM

void WorldSystem::render()
{
	//for all players
		
	//render tiles to camera based on player location
	
	//only render tiles near player
	
	if(room_one.in_active_use)
	{
		for(size_t i = 0; i < m_camera_manager_ptr->screens.size(); i++)
		{
			if(m_camera_manager_ptr->screens[i].in_active_use)
			{
				if(m_camera_manager_ptr->screens[i].camera_ptr)
				{
					//#ifdef DEBUG_CAMERA_SYSTEM
					//std::cout << " camera active status: " << m_camera_manager_ptr->screens[i].camera_ptr->GetCameraActiveStatus() << std::endl;
					//#endif
					
					//if camera is active and has the same id as the world
					if( room_one.world_id == m_camera_manager_ptr->screens[i].camera_ptr->GetWorldID()  )
					{
						#ifdef DEBUG_CAMERA_SYSTEM
						std::cout << "\n\nRendering Tiles in world system\n\n";
						
						std::cout << "screen " << i << std::endl;
						
						std::cout << "camera :" 
						<< m_camera_manager_ptr->screens[i].screen_camera.x << " , " << 
						m_camera_manager_ptr->screens[i].screen_camera.y << " , " <<
						m_camera_manager_ptr->screens[i].screen_camera.width << " , " <<
						m_camera_manager_ptr->screens[i].screen_camera.height
						<< std::endl;
						#endif
						
						RenderLevelMapRelativeToCameraAndScreen(&room_one,
																m_camera_manager_ptr->screens[i].screen_camera,
																m_camera_manager_ptr->screens[i].screen_rect);
					}
				}
				else
				{
					std::cout << "Uninitialized camera for active screen!\n";
				}
			}
			
		}
		
		
	}
	
}
