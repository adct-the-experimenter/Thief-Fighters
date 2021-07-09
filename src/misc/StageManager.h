#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include "level_maps.h"
#include <vector>
#include <array>

struct StageInfo
{
	//stage profile info 
	std::string name;
	std::string profileImageFilePath;
	
	//paths to stage gameplay info
	std::string collisionMapFilePath;
	std::string textureFilePath;
	std::string playerStartFilePath;
	
	bool initialized = false;
	
	//scrolling background info
	bool scrolling = false;
	std::string scrollingBGTextureFilePath;
	float scrollSpeed = 0;
};


class StageManager
{
public:
	
	StageManager();
	
	bool LoadLevel(int level);
	void FreeCurrentLoadedLevel();
	
	void PlacePlayersInStage(std::int8_t num_players);
	
	bool LoadStageInfoFromXML();
	
	bool GetStagesInfoLoadedBool();
	
	int GetNumberOfStagesInitialized();
	
private:
	
	uint8_t max_num_rect;
	
	//information on 8 stages
	std::array <StageInfo,8> stages_info;
	
	//bool to indicate stages info has been loaded already.
	//prevents reloading stages after restart
	bool stages_info_loaded;
	
	bool LoadStageProfiles();
	bool LoadStageGamePlayInfo();
	
	int num_stages_initialized;
	
};


#endif
