#include "world_editor.h"

#include "pugixml.hpp"

#include <cmath>

World world_one;

WorldEditor::WorldEditor()
{
	m_tiles_placement_xml_file_path = "";
	m_tilesheet_xml_file_path = "";
	
	m_click = false;
	
	m_tile_selector.current_tile_index = 0;
	m_tile_selector.current_tile = &m_tile_selector.select_tiles[0];
	
	m_tile_selector.x = 30;
	m_tile_selector.y = 100;
	
	m_save_button_rect = {130,50,60,30};
	
	//tile placement area
	m_tiles_startx = 250;
	m_tiles_starty = 200;
}

WorldEditor::~WorldEditor()
{
	
}

void WorldEditor::handle_input_mouse()
{
	mouse_click_bool = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
			
	if(mouse_click_bool)
	{
		mouse_x = GetMousePosition().x;
		mouse_y = GetMousePosition().y;
	}
}

static bool MouseInBox(float mouseX, float mouseY, Rectangle box)	
{
	if( mouseX < box.x)
	{
		return false;
	}
	else if(mouseX > box.x + box.width)
	{
		return false;
	}
	
	if(mouseY < box.y)
	{
		return false;
	}
	else if(mouseY > box.y + box.height)
	{
		return false;
	}
	
	return true;
}

void WorldEditor::logic()
{
	// if mouse click
	if(mouse_click_bool)
	{
		//if mouse click on the tile selection relative to camera
		//change tile selection relative to camera
		for(size_t i = 0; i < m_tile_selector.select_tiles.size(); i++)
		{
			Rectangle box = {m_tile_selector.select_tiles[i].select_box.x,
							m_tile_selector.select_tiles[i].select_box.y,
							30,30};
							
			//set tile type in array
			if(MouseInBox(mouse_x,mouse_y,box))
			{
				//set current tile to the one that was clicked on. use index of vector to set pointer
				m_tile_selector.current_tile = &m_tile_selector.select_tiles[i];
				m_tile_selector.current_tile_index = i;
			}
			
		}
		
		//else 
		//if mouse click on map relative to camera
		//place tile relative to camera
		//if area for placing tiles was clicked on
		
		Rectangle* camera_ptr = m_camera_manager_ptr->lead_cameras[0].GetCameraRectPointer();
		
		size_t num_tiles_horizontal = 220;
		
		//render 9 rows of tiles
	
		size_t start_tiles[12];
		size_t end_tiles[12];
		
		//get index based on top left corner of camera
		size_t horiz_index = trunc(camera_ptr->x / 30 );
		size_t vert_index = trunc( camera_ptr->y / 30 ) * num_tiles_horizontal;
		
		//initialize start tiles
		for(size_t i = 0; i < 12; i++)
		{
			start_tiles[i] = horiz_index + vert_index + i*num_tiles_horizontal;
			if(start_tiles[i] >= world_one.tiles_vector.size()){start_tiles[i] = world_one.tiles_vector.size() - 2;}
		}
		
		//get camera width offset
		size_t camera_offset_width = trunc( camera_ptr->width / 30 );
		
		//initialize end tiles
		for(size_t i = 0; i < 12; i++)
		{
			end_tiles[i] = start_tiles[i] + camera_offset_width;
			if(end_tiles[i] >= world_one.tiles_vector.size()){end_tiles[i] = world_one.tiles_vector.size() - 1;}
		}
		
		
		for(size_t i = 0; i < 12; i++)
		{
			for(size_t tile_index = start_tiles[i]; tile_index < end_tiles[i]; tile_index++)
			{
				//std::cout << "tile_index: " << tile_index << std::endl;
				
				Rectangle box = {world_one.tiles_vector[tile_index].x,
							world_one.tiles_vector[tile_index].y,
							30,30};
							
				box.x += m_tiles_startx;
				//std::cout << "\nmouse : " << mouse_x << " , " << mouse_y << std::endl;
				//std::cout << "\nbox : " << box.x << " , " << box.y << std::endl;
				
				float editor_mouse_x  = mouse_x + camera_ptr->x;
				float editor_mouse_y  = mouse_y + camera_ptr->y;
				
				//std::cout << "\neditor mouse : " << editor_mouse_x << " , " << editor_mouse_y << std::endl;
				
				//set tile type in array
				if(MouseInBox(editor_mouse_x,editor_mouse_y,box))
				{
					std::cout << "Tile clicked on!\n";
					std::cout << "\nmouse : " << mouse_x << " , " << mouse_y << std::endl;
					std::cout << "\nbox : " << box.x << " , " << box.y << std::endl;
					if(m_tile_selector.current_tile)
					{
						world_one.tiles_vector[tile_index].frame_clip_ptr = &m_tile_selector.current_tile->frame_clip;
						world_one.tiles_vector[tile_index].type = m_tile_selector.current_tile->type;
						world_one.tiles_vector[tile_index].tile_id = m_tile_selector.current_tile->tile_number;
					}
					
				}
			}
			
			
		}
		
				
		//if mouse click on the save button
		if(MouseInBox(mouse_x,mouse_y,m_save_button_rect))
		{
			//Save file 
			WorldEditor::SaveDataToXMLFile(m_tiles_placement_xml_file_path);
		}
	}
		
	
}

void WorldEditor::RenderLevelMapRelativeToCamera(World* world_ptr,Rectangle& camera)
{
	
	//number of tiles in a row
	size_t num_tiles_horizontal = 220;
	
	//render 9 rows of tiles
	
	size_t start_tiles[12];
	size_t end_tiles[12];
	
	//get index based on top left corner of camera
	size_t horiz_index = trunc(camera.x / 30 );
	size_t vert_index = trunc( camera.y / 30 ) * num_tiles_horizontal;
	
	//initialize start tiles
	for(size_t i = 0; i < 12; i++)
	{
		start_tiles[i] = horiz_index + vert_index + i*num_tiles_horizontal;
		if(start_tiles[i] > world_ptr->tiles_vector.size()){start_tiles[i] = world_ptr->tiles_vector.size() - 1;}
	}
	
	//get camera width offset
	//added 2 tiles to prevent tearing when player is walking
	size_t camera_offset_width = trunc( camera.width / 30 ) + 2;
	
	//initialize end tiles
	for(size_t i = 0; i < 12; i++)
	{
		end_tiles[i] = start_tiles[i] + camera_offset_width;
		if(end_tiles[i] >= world_one.tiles_vector.size()){end_tiles[i] = world_one.tiles_vector.size() - 1;}
	}
	
	
	for(size_t i = 0; i < 12; i++)
	{
		for(size_t tile_index = start_tiles[i]; tile_index < end_tiles[i]; tile_index++)
		{
			
			Vector2 pos = {world_ptr->tiles_vector.at(tile_index).x - camera.x ,world_ptr->tiles_vector.at(tile_index).y - camera.y };
			pos.x += m_tiles_startx;
			
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

void WorldEditor::render()
{
	
	//render tiles
	//for(size_t i = 0; i < m_camera_manager_ptr->lead_cameras.size(); i++)
	//{
	//	if(m_camera_manager_ptr->lead_cameras[i].GetCameraActiveStatus())
	//	{
	//		RenderLevelMapRelativeToCamera(&world_one,*m_camera_manager_ptr->lead_cameras[i].GetCameraRectPointer());
	//	}
		
	//}
	RenderLevelMapRelativeToCamera(&world_one,*m_camera_manager_ptr->lead_cameras[0].GetCameraRectPointer());
	
	
	//render tile box 
	for(size_t i = 0; i < m_tile_selector.select_tiles.size(); i++)
	{
		Vector2 pos = {m_tile_selector.select_tiles[i].select_box.x,
						m_tile_selector.select_tiles[i].select_box.y};
		
		//render texture
		DrawTextureRec(world_one.tilesheet_texture, 
						   m_tile_selector.select_tiles[i].frame_clip, 
						   pos, 
						   WHITE);
						   
		//if a tile is selected render it with yellow rectangle beside it
		if(i == m_tile_selector.current_tile_index)
		{
			DrawRectangleRec((Rectangle){pos.x - 30,pos.y + 10,30/2,30/2}, YELLOW);
		}
	}
	
	//render save button
	
	DrawRectangleRec(m_save_button_rect, GRAY);
	DrawText("Save",m_save_button_rect.x,m_save_button_rect.y,12, BLACK);
	
}

void WorldEditor::SetLevelFilesToEdit(std::string tiles_placement_xml_file_path,std::string tilesheet_xml_file_path)
{
	m_tiles_placement_xml_file_path = tiles_placement_xml_file_path;
	m_tilesheet_xml_file_path = tilesheet_xml_file_path;
}

bool WorldEditor::LoadDataBasedOnTilesheetDescription(std::string filepath)
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
    
    
    //load tile sheet
    world_one.tilesheet_texture = LoadTexture(tilesheet_path.c_str());
        
    //set up tile selector based on data
    pugi::xml_node tileRoot = root.child("Tiles");
    
    size_t iterator = 0;
	//go through each tile type in tiles node
	for (pugi::xml_node tile_node = tileRoot.first_child(); tile_node; tile_node = tile_node.next_sibling())
	{	
		SelectableTile sTile;
		
		//set type
		TileType type = TileType::NONE;
		
		std::string valString = tile_node.attribute("type").value();
		//std::cout << valString << std::endl;
		
		if(valString ==  "PUSHBACK"){type = TileType::PUSH_BACK; }
		else if(valString ==  "BACKGROUND"){ type = TileType::BACKGROUND; }
		else{std::cout << "Tile type not handled!" << "i:" << iterator << std::endl; }
		
		sTile.type = type;
		
		//set frame clip
		
		std::uint32_t x,y,width,height;
		
		valString = tile_node.attribute("x").value();
		x = atoi(valString.c_str());
		
		valString = tile_node.attribute("y").value();
		y = atoi(valString.c_str());
		
		//valString = tile_node.attribute("width").value();
		//width = atoi(valString.c_str());
		
		//valString = tile_node.attribute("height").value();
		//height = atoi(valString.c_str());
		
		sTile.frame_clip = {x,y,30,30};
		
		//set tile number
		valString = tile_node.attribute("id").value();
		sTile.tile_number = atoi(valString.c_str());
		
		//push into frame clip map
		frame_clip_map[sTile.tile_number] = sTile.frame_clip;
		
		//push into vector
		m_tile_selector.select_tiles.push_back(sTile);
		
	}
	
	//set select box for each select tile
	
	std::uint32_t x_offset = 20;
	std::uint32_t y_offset = 10;
	
	for(size_t i = 0; i < m_tile_selector.select_tiles.size(); i++)
	{
		 
		if((i+1) % 3 == 0)
		{
			x_offset = 30;
			y_offset += m_tile_selector.select_tiles[i].frame_clip.height + 10;
		}
		
		m_tile_selector.select_tiles[i].select_box.x = m_tile_selector.x + x_offset;
		m_tile_selector.select_tiles[i].select_box.y = m_tile_selector.y + y_offset;
		m_tile_selector.select_tiles[i].select_box.width = m_tile_selector.select_tiles[i].frame_clip.width;
		m_tile_selector.select_tiles[i].select_box.height = m_tile_selector.select_tiles[i].frame_clip.height;
		
		x_offset += m_tile_selector.select_tiles[i].frame_clip.width;
	}
	
	
	return true;	
}

bool WorldEditor::LoadDataFromXMLFile(std::string mapFilePath, std::string tilesheetDescriptionFilePath)
{
	//read tilesheet description xml file for tile parameters
	if(!WorldEditor::LoadDataBasedOnTilesheetDescription(tilesheetDescriptionFilePath))
	{
		std::cout << "Failed to read tilesheet description xml!\n";
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
			
			//set frame clip pointer based on tile number
			
			tile.frame_clip_ptr = &frame_clip_map[tile.tile_id];
			
			//assign to vector
			world_one.tiles_vector[iterator] = tile;
			
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
		for(size_t i = 0; i < world_one.tiles_vector.size(); i++)
		{
			if(i % num_tiles_horiz == 0 && i != 0)
			{
				x_offset = 0;
				y_offset += tile_height;
			}
			
			world_one.tiles_vector[i].x = x_offset;
			world_one.tiles_vector[i].y = y_offset;
			
			x_offset += tile_width;
		}
		
	}
	
	return true;
    
}

bool WorldEditor::LoadLevel()
{
	
	size_t num_tiles_horiz = 220;
	size_t square_area = num_tiles_horiz * num_tiles_horiz;
	
	world_one.tiles_vector.resize(square_area);
	
	world_one.in_active_use = true;
	
	//load the xml file containing info on tile level positions and tile types
	
	
	//read xml file containing tile sheet path and tile positions
	//assuming tile is uniform size of 30x30 pixels
	//load texture for tile sheet
	//load the tile sheet clips
	
	if(!WorldEditor::LoadDataFromXMLFile(m_tiles_placement_xml_file_path, m_tilesheet_xml_file_path))
	{
		std::cout << "\nFailed to load data from xml files!\n";
		return false;
	}
	
	return true;
}

bool WorldEditor::MakeLevel()
{
	//read tilesheet description xml file for tile parameters
	if(!WorldEditor::LoadDataBasedOnTilesheetDescription( m_tilesheet_xml_file_path))
	{
		std::cout << "Failed to read tilesheet description xml!\n";
		return false;
	}
	
	world_one.in_active_use = true;
	
	size_t num_tiles_horiz = 220;
	size_t square_area = num_tiles_horiz * num_tiles_horiz;
	
	world_one.tiles_vector.resize(square_area);
	
	//number of horizontal tiles
	//10 sections * 640 pixels/section / 30 pixels/tile ~= 220
	size_t tile_height = 30;
	size_t tile_width = 30;
	
	
	size_t x_offset = 0;
	size_t y_offset = 0;
	
	size_t flat_platform_tile = world_one.tiles_vector.size() - num_tiles_horiz - 1;
	
	//initialize tile position and tile type
	for(size_t i = 0; i < world_one.tiles_vector.size(); i++)
	{
		if(i % num_tiles_horiz == 0 && i != 0)
		{
			x_offset = 0;
			y_offset += tile_height;
		}
		
		world_one.tiles_vector[i].x = x_offset;
		world_one.tiles_vector[i].y = y_offset;
		
		world_one.tiles_vector[i].tile_id = 0;
		world_one.tiles_vector[i].type = TileType::BACKGROUND;
		if(i > flat_platform_tile)
		{
			world_one.tiles_vector[i].tile_id = 1;
			world_one.tiles_vector[i].type = TileType::PUSH_BACK;
		}
		
		world_one.tiles_vector[i].frame_clip_ptr = &frame_clip_map[world_one.tiles_vector[i].tile_id];
		
		x_offset += tile_width;
	}
	
	return true;
}

void WorldEditor::SaveDataToXMLFile(std::string filepath)
{
	std::cout << "Creating level map xml file...\n";
	
	// Generate new XML document within memory
    pugi::xml_document doc;
    
    // Generate XML declaration
    auto declarationNode = doc.append_child(pugi::node_declaration);
    declarationNode.append_attribute("version")    = "1.0";
    declarationNode.append_attribute("encoding")   = "ISO-8859-1";
    declarationNode.append_attribute("standalone") = "yes";
    
    // A valid XML doc must contain a single root node of any name
    auto root = doc.append_child("RootMap");
    
    
    //create tiles node
    pugi::xml_node tilesNode = root.append_child("Tiles");
    
    //save tile type
		
	for(size_t i = 0; i < world_one.tiles_vector.size(); i++)
	{
		 // Add child based on tile type
		pugi::xml_node nodeChild = tilesNode.append_child("Tile");
		
		switch(world_one.tiles_vector[i].type)
		{
			case TileType::PUSH_BACK:{ nodeChild.append_attribute("type").set_value("PUSHBACK"); break;}
			case TileType::BACKGROUND:{ nodeChild.append_attribute("type").set_value("BACKGROUND"); break;}
			default:{nodeChild.append_attribute("type").set_value("NONE"); std::cout << "Tile type not handled! Placing None.\n"; break;}
		}
		
		nodeChild.append_attribute("id").set_value( std::to_string(world_one.tiles_vector[i].tile_id).c_str() );
		
	}

    
    // Save XML tree to file.
    // Remark: second optional param is indent string to be used;
    // default indentation is tab character.
	bool saveSucceeded = doc.save_file(filepath.c_str(), PUGIXML_TEXT("  "));
	
	if(saveSucceeded)
	{
		std::cout << "Finished saving level map xml file!\n";
	}
	else
	{
		std::cout << "Error! Save was not successful!\n";
	}
	
}

void WorldEditor::SaveLevel()
{
	//save world info to xml file
	
	//save tile level positions and tile types
}

void WorldEditor::FreeLevel()
{
	UnloadTexture(world_one.tilesheet_texture);
}

void WorldEditor::SetPointerToCameraManager(CameraManager* cam_manager_ptr){m_camera_manager_ptr = cam_manager_ptr;}
