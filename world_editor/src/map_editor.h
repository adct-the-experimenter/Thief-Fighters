#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "misc/level_maps.h"

class MapEditor
{
public:
	
	//function that chooses current map unit to edit in world map
	void ChooseThisMapUnitToEdit(size_t index);
	
	//function that takes in room and draws map unit of room
	void DrawCurrentMapUnitFromRoomTiles(Room* room_ptr);
	
	//function to set pointer to world map
	void SetPointerToWorldMap(WorldMap* world_map_ptr);
	
	//function to save world map texture to image file
	void SaveWorldMapToImageFile();
	
private:

	size_t m_current_map_unit_index;
	
	WorldMap* m_world_map_ptr;
};

#endif
