
#ifndef CHAR_SELECTOR_H
#define CHAR_SELECTOR_H

#include <cstdint>
#include <vector>
#include <array>

#include "raylib.h"
#include "core/ControllerInput.h"
#include "core/KeyboardInput.h"
#include "core/entity.h"

#include "misc/char_media.h" //for texture

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
	
	struct CharacterProfileWall
	{
		std::array <Texture2D,8> char_profiles_textures;
		std::array <Rectangle,8> char_profiles_rects;
		std::array <uint8_t,8> player_selection;
		
	};
	
	struct FighterBox
	{
		Rectangle background_box;
		
		std::string player_num_string;
		Rectangle player_num_rect;
		
		//slots: 0 = character, 1 = special_power
		
		//current slot
		std::uint8_t current_slot = 0;
		
		std::uint8_t char_choice = 0;
		std::uint8_t special_power_choice = 0;
		
		Rectangle char_slot_rect;
		
		Rectangle special_power_slot_rect;
		
		bool confirm_char_choice = false;		
		bool confirm_power_choice = false;
	};
	
	void Reset();
	
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
	
	//wall of character profiles
	CharacterProfileWall char_profile_wall;
	
};

#endif
