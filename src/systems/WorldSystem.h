
#ifndef WORLD_SYSTEM_H
#define WORLD_SYSTEM_H

#include "core/system.h"

#include "misc/level_maps.h"

class WorldSystem : public System
{
public:
	
	//function to initialize world system
	void Init();
	
	//function to load filepaths for the worlds
	bool LoadWorldFilepaths();
	
	//function to load level
	bool LoadWorldLevel(std::uint8_t level_num);
	
	//function to free level
	void FreeWorldLevel(std::uint8_t level_num);
	
	

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
	
	bool LoadDataBasedOnTilesheetDescription(World* world_ptr,std::string filepath);
	
	bool LoadDataFromXMLFile(World* world_ptr,std::string mapFilePath, std::string tilesheetDescriptionFilePath);
};

#endif
