#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

#include "level_maps.h"
#include <vector>


class StageManager
{
public:
	
	StageManager();
	
	bool LoadLevel(std::uint16_t level);
	void FreeCurrentLoadedLevel();
	
	void PlacePlayersInStage(std::int8_t num_players);
	
private:
	
	
	
	
};


#endif
