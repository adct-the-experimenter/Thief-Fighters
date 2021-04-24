
#ifndef STAGE_SELECTOR_H
#define STAGE_SELECTOR_H

#include <cstdint>
#include <vector>
#include <array>

#include "raylib.h"
#include "core/ControllerInput.h"
#include "core/KeyboardInput.h"
#include "core/entity.h"



	
struct NameSlot
{
	//text slot with a limit of 15 characters
	char text[15];
	
	Rectangle textBox;
	size_t letterCount = 0;
};

struct RenderModelSlot
{
	Rectangle frame_clip = {0,0,30,30};
	
	size_t style_choice = 0;
	size_t color_choice = 0;
};

struct StageBox
{
	
	NameSlot name_slot;
	
	//hair = 0, head=1,eyes=2,upper clothing = 3, lower clothing = 4, shoes=5
	RenderModelSlot render_slot;
		
	bool confirm_selection = false;
};

class StageSelector
{
public:
	StageSelector();
	~StageSelector();
	
	void Init(std::uint8_t num_players);
	
	//game loop functions
	
	void handle_input(ControllerInput& controller_input, KeyboardInput& key_input);
	
	void logic();
	
	void render();
	
	void sound();
	
	bool MoveToNextStateBool();
	
	std::uint8_t StageSelected();	
	
private:

	//vector to hold pointers to player entities
	std::vector <Entity*> player_entities_vec;
	
	//vector to hold a character box for each player
	std::vector <StageBox> stage_boxes;
	
	std::uint8_t m_stage_selected;
	
	//confirmation for whether stage was selected
	bool stage_confirmation;
	
	//bool to indicate whether to move to the next state
	bool move_next_state;
	
	//input specific handler functions
	void handle_controller_input(ControllerInput& input);
	void handle_keyboard_input(KeyboardInput& input);
	
	std::uint8_t m_num_stages;
};

#endif
