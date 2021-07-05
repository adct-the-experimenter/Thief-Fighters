
#ifndef WORLD_SYSTEM_H
#define WORLD_SYSTEM_H

#include "core/system.h"

#include "misc/level_maps.h"
#include "misc/camera.h"

class WorldSystem : public System
{
public:
	
	//function to initialize world system, returns true of succesful, false otherwise
	bool Init();
	
	//function to load filepaths for the worlds
	bool LoadWorldFilepaths();
	
	//function to set pointer to camera manager
	void SetPointerToCameraManager(CameraManager* cam_manager_ptr);
	
	//function to free resources that the world system uses
	void FreeResources();

//gameplay loop functoins
	void logic(float& dt);
	
	void render();
	
	
private:
	
	//path to xml file that contains file paths to world assets
	std::string m_levels_filepaths_xml;
	
	//paths to level xml files
	std::array <std::string,4> m_world_levels_filepaths;
	
	//paths to tilesheet xml files for levels
	std::array <std::string,4> m_tilesheet_filepaths;
	
	//pointer to camera manager
	CameraManager* m_camera_manager_ptr;
	
	//function to load tile sheet information
	bool LoadDataBasedOnTilesheetDescription(World* world_ptr,std::string filepath);
	
	//function to load level information
	bool LoadDataFromXMLFile(World* world_ptr,std::string mapFilePath, std::string tilesheetDescriptionFilePath);
	
	//function to load level
	bool LoadWorldLevel(World* world_ptr, std::uint8_t level_num);
	
	//function to free level
	void FreeWorldLevel(World* world_ptr);
};

#endif
