
#include "map_editor.h"


void MapEditor::ChooseThisMapUnitToEdit(size_t index){m_current_map_unit_index = index;}
	
void MapEditor::DrawCurrentMapUnitFromRoomTiles(Room* room_ptr)
{
	
}
	
void MapEditor::SetPointerToWorldMap(WorldMap* world_map_ptr){m_world_map_ptr = world_map_ptr;}
