#ifndef WORLD_EDITOR_H
#define WORLD_EDITOR_H

#include <cstdint>

#include <array>

#include "raylib.h"

#include <vector>
#include <string>

#include "misc/level_maps.h"

#include <map>

#include "misc/camera.h"

struct SelectableTile
{
	std::uint32_t tile_number;
	Rectangle frame_clip;
	Rectangle select_box;
	TileType type;
};

struct TileBox
{
	//position of tile box
	std::uint32_t x;
	std::uint32_t y;
	
	std::vector <SelectableTile> select_tiles;
	SelectableTile* current_tile;
	size_t current_tile_index;
};

class WorldEditor
{

public:

	WorldEditor();
	~WorldEditor();

//game loop
	void handle_input_mouse();
	
	void logic();
	
	void render();

//editor

	//function to set files
	void SetLevelFilesToEdit(std::string tiles_placement_xml_file_path,std::string tilesheet_xml_file_path);
	
	//function to load level
	bool LoadLevel();
	
	//function to make blank default level
	bool MakeLevel();
	
	//function to save level
	void SaveLevel();
	
	//function to free level edited
	void FreeLevel();
	
	void SetPointerToCameraManager(CameraManager* cam_manager_ptr);
	
private:
	
	
	//new room button variables
	Rectangle m_new_room_button_rect;
	
	//save button variables
	Rectangle m_save_button_rect;
	
//Tile selection

	//file path to tile placement info xml file
	std::string m_tiles_placement_xml_file_path;
	
	//file path to tile sheet used
	std::string m_tilesheet_xml_file_path;

	//box that allows selecting from tiles
	TileBox m_tile_selector;
	
	//last mouse click position
	float m_mouseX,m_mouseY;
	bool m_click;
	
	
	//map to keep track of the tile number of a tile from tile sheet.
	std::map <std::uint32_t, Rectangle> frame_clip_map;
	
	bool mouse_click_bool;
	float mouse_x;
	float mouse_y;
	
	//for pushing the rest of the map to right to select tiles
	float m_tiles_startx;
	float m_tiles_starty;
	
//XML file info handling

	bool LoadDataBasedOnTilesheetDescription(std::string filepath);
	
	bool LoadDataFromXMLFile(std::string mapFilePath, std::string tilesheetDescriptionFilePath);
	
	void SaveDataToXMLFile(std::string filepath);

//camera
	CameraManager* m_camera_manager_ptr;
	
	void RenderLevelMapRelativeToCamera(World* world_ptr,Rectangle& camera);
	
	
};


#endif
