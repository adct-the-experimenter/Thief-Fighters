
#ifndef CHAR_SELECTOR_H
#define CHAR_SELECTOR_H

#include <cstdint>
#include <vector>
#include <array>

#include "raylib.h"
#include "core/ControllerInput.h"
#include "core/KeyboardInput.h"
#include "core/entity.h"

#include "misc/media.h" //for texture

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
	
	RequestedCharacters& GetRequestedCharacters();
	
	struct FighterBox
	{
		Rectangle background_box;
		
		std::string player_num_string;
		Rectangle player_num_rect;
		
		//slots: 0 = character, 1 = special_power, 2 = confirm
		
		//current slot
		std::uint8_t current_slot = 0;
		
		std::uint8_t char_choice = 0;
		std::uint8_t special_power_choice = 0;
		
		Rectangle char_slot_rect;
		
		Rectangle special_power_slot_rect;
		
		Rectangle confirm_selection_rect;
		
		bool confirm_selection = false;
	};
	
private:

	//vector to hold pointers to player entities
	std::vector <Entity*> player_entities_vec;
	
	//vector to hold a character box for each player
	std::vector <FighterBox> fighter_boxes;
		
	//vector to hold bools for character creation confirmation for players
	std::vector <bool> char_confirmations;
	
	//bool to indicate whether to move to the next state
	bool move_next_state;
	
	//input specific handler functions
	void handle_controller_input(ControllerInput& input);
	void handle_keyboard_input(KeyboardInput& input);
	
	std::uint8_t m_num_fighters;	
	std::uint8_t m_num_special_powers;
	
	RequestedCharacters m_req_char;
};

#endif
