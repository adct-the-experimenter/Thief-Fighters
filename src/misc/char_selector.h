
#ifndef CHAR_SELECTOR_H
#define CHAR_SELECTOR_H

#include <cstdint>
#include <vector>
#include <array>

#include "raylib.h"
#include "core/ControllerInput.h"
#include "core/KeyboardInput.h"
#include "core/entity.h"



	


class CharacterSelector
{
public:
	CharacterSelector();
	~CharacterSelector();
	
	void Init(std::vector <Entity> *entities_vec_ptr, std::uint8_t num_players);
	
	//game loop functions
	
	void handle_input(ControllerInput& controller_input, KeyboardInput& key_input);
	
	void logic();
	
	void render();
	
	void sound();
	
	bool MoveToNextStateBool();
	
	std::uint8_t name_slot = 0;
	std::uint8_t job_slot = 1;
	std::uint8_t render_slot = 2; //index for first render slot
	std::uint8_t last_slot = 7; //index for the last slot
	

	struct ChoiceIndicator
	{
		Rectangle frame_clip = {0,0,30,30};
		
	};

	struct FighterBox
	{
				
		ChoiceIndicator choice_indicator;
			
		bool confirm_selection = false;
	};
	
private:

	//vector to hold pointers to player entities
	std::vector <Entity*> player_entities_vec;
	
	//vector to hold a character box for each player
	std::vector <FighterBox> fighter_boxes;
	
	//array to hold color choices
	std::array <Color,8> colors;
	
	//vector to hold bools for character creation confirmation for players
	std::vector <bool> char_confirmations;
	
	//bool to indicate whether to move to the next state
	bool move_next_state;
	
	//input specific handler functions
	void handle_controller_input(ControllerInput& input);
	void handle_keyboard_input(KeyboardInput& input);
	
	std::uint8_t m_num_fighters;
};

#endif
