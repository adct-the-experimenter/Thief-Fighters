#include "world_editor.h"

#include "pugixml.hpp"

World world_edited;

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
}

WorldEditor::~WorldEditor()
{
	
}


void WorldEditor::logic()
{
	// if mouse click
		
		//if mouse click on the tile selection relative to camera
		//change tile selection relative to camera
		
		//else 
		//if mouse click on map relative to camera
		//place tile relative to camera
		
		//if mouse click on the save button
	
}

static void RenderLevelMapRelativeToCamera(World* world_ptr,Rectangle& camera)
{
	bool render = true;
	
	
	if(render)
	{
		for(size_t i = 0; i < world_ptr->tiles_vector.size(); i++)
		{
			bool renderTile = false;
			
			if( (world_ptr->tiles_vector.at(i).x >= camera.x) && 
				(world_ptr->tiles_vector.at(i).x <= camera.x + camera.width) &&
				(world_ptr->tiles_vector.at(i).y >= camera.y) &&
				(world_ptr->tiles_vector.at(i).y <= camera.y + camera.height))
			{
				renderTile = true;
			}
			
			if( renderTile )
			{
				
				Vector2 pos = {world_ptr->tiles_vector.at(i).x - camera.x,world_ptr->tiles_vector.at(i).y - camera.y};
				if(world_ptr->tiles_vector.at(i).frame_clip_ptr)
				{
					DrawTextureRec(world_ptr->tilesheet_texture, 
							   *world_ptr->tiles_vector.at(i).frame_clip_ptr, 
							   pos, 
							   WHITE);
				}
			}
			
		}
	}
}

void WorldEditor::render()
{
	
	//render tiles
	//RenderLevelMapRelativeToCamera(&world_edited,Rectangle& camera);
	
	//render tile box 
	for(size_t i = 0; i < m_tile_selector.select_tiles.size(); i++)
	{
		Vector2 pos = {m_tile_selector.select_tiles[i].select_box.x,
						m_tile_selector.select_tiles[i].select_box.y};
		
		//render texture
		DrawTextureRec(world_edited.tilesheet_texture, 
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
    world_edited.tilesheet_texture = LoadTexture(tilesheet_path.c_str());
        
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
			world_edited.tiles_vector[iterator] = tile;
			
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
		for(size_t i = 0; i < world_edited.tiles_vector.size(); i++)
		{
			if(i % num_tiles_horiz == 0)
			{
				x_offset = 0;
				y_offset += tile_height;
			}
			
			world_edited.tiles_vector[i].x = x_offset;
			world_edited.tiles_vector[i].y = y_offset;
			
			x_offset += tile_width;
		}
		
	}
	
	return true;
    
}

bool WorldEditor::LoadLevel()
{
	
	size_t num_tiles_horiz = 220;
	size_t square_area = num_tiles_horiz * num_tiles_horiz;
	
	world_edited.tiles_vector.resize(square_area);
	
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
	
}

bool WorldEditor::MakeLevel()
{
	//read tilesheet description xml file for tile parameters
	if(!WorldEditor::LoadDataBasedOnTilesheetDescription( m_tilesheet_xml_file_path))
	{
		std::cout << "Failed to read tilesheet description xml!\n";
		return false;
	}
	
	size_t num_tiles_horiz = 220;
	size_t square_area = num_tiles_horiz * num_tiles_horiz;
	
	world_edited.tiles_vector.resize(square_area);
	
	//number of horizontal tiles
	//10 sections * 640 pixels/section / 30 pixels/tile ~= 220
	size_t tile_height = 30;
	size_t tile_width = 30;
	
	bool row_platforms_bool = false;
	
	size_t x_offset = 0;
	size_t y_offset = 0;
	
	//initialize tile position and tile type
	for(size_t i = 0; i < world_edited.tiles_vector.size(); i++)
	{
		if(i % num_tiles_horiz == 0)
		{
			x_offset = 0;
			y_offset += tile_height;
			row_platforms_bool = !row_platforms_bool;
		}
		
		world_edited.tiles_vector[i].x = x_offset;
		world_edited.tiles_vector[i].y = y_offset;
		
		world_edited.tiles_vector[i].tile_id = 0;
		world_edited.tiles_vector[i].type = TileType::BACKGROUND;
		if(row_platforms_bool)
		{
			world_edited.tiles_vector[i].tile_id = 1;
			world_edited.tiles_vector[i].type = TileType::PUSH_BACK;
		}
		
		world_edited.tiles_vector[i].frame_clip_ptr = &frame_clip_map[world_edited.tiles_vector[i].tile_id];
		
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
    
    //save path to tile sheet
    //pugi::xml_node tilesheetNode = root.append_child("Tilesheet");
    
    //tilesheetNode.append_attribute("path").set_value( m_tilesheet_path.c_str() );
    
    //save number of tiles in level
	//std::cout << "Number of tiles: " << world_edited.size() << std::endl;
	
	//pugi::xml_node numTilesNode = root.append_child("NumberOfTiles");
	//numTilesNode.append_attribute("num").set_value( std::to_string(m_tiles_vec.size()).c_str() );
	
	//save level dimensions
    //pugi::xml_node dimensionsNode = root.append_child("Dimensions");
    
    //dimensionsNode.append_attribute("width").set_value( std::to_string(int(m_levelWidth)).c_str() );
    //dimensionsNode.append_attribute("height").set_value( std::to_string(int(m_levelHeight)).c_str() );
    
    //save tile width tile height
    //pugi::xml_node tile_dimensionsNode = root.append_child("TileDimensions");
    
    //tile_dimensionsNode.append_attribute("width").set_value( std::to_string(int(m_tile_width)).c_str() );
    //tile_dimensionsNode.append_attribute("height").set_value( std::to_string(int(m_tile_height)).c_str() );
    
    
    //create tiles node
    pugi::xml_node tilesNode = root.append_child("Tiles");
    
    //save tile type
		
	for(size_t i = 0; i < world_edited.tiles_vector.size(); i++)
	{
		 // Add child based on tile type
		pugi::xml_node nodeChild = tilesNode.append_child("Tile");
		
		switch(world_edited.tiles_vector[i].type)
		{
			case TileType::PUSH_BACK:{ nodeChild.append_attribute("type").set_value("PUSHBACK"); break;}
			case TileType::BACKGROUND:{ nodeChild.append_attribute("type").set_value("BACKGROUND"); break;}
			default:{nodeChild.append_attribute("type").set_value("NONE"); std::cout << "Tile type not handled! Placing None.\n"; break;}
		}
		
		nodeChild.append_attribute("id").set_value( std::to_string(world_edited.tiles_vector[i].tile_id).c_str() );
		
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
	UnloadTexture(world_edited.tilesheet_texture);
}
