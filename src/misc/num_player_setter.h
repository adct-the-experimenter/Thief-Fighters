
#ifndef NUM_PLAYER_SETTER_H
#define NUM_PLAYER_SETTER_H

#include <cstdint>

#include <array>

#include "raylib.h"
#include "core/ControllerInput.h"
#include "core/KeyboardInput.h"
#include "core/entity.h"

#include <vector>
#include <string>

struct NumbersBox
{	
	
	Rectangle render_rect;
	std::string text;
	
};

class NumPlayerSetter
{
public:
	NumPlayerSetter();
	~NumPlayerSetter();
	
	void Init();
	
	//game loop functions
	
	void handle_input(ControllerInput& controller_input, KeyboardInput& key_input);
	
	void logic();
	
	void render();
	
	void sound();
	
	bool MoveToNextStateBool();
	
	void CreatePlayerEntities(std::vector <Entity> *entities_vec_ptr);
	
	std::int8_t GetNumberOfPlayers();
	
private:
	
	//vector to hold a number of players box
	std::array <NumbersBox,8> num_players_boxes;
	
	size_t m_current_slot;
	bool m_confirm_selection;
	
	//bool to indicate whether to move to the next state
	bool move_next_state;
	
	//input specific handler functions
	void handle_controller_input(ControllerInput& input);
	void handle_keyboard_input(KeyboardInput& input);
	
	std::int8_t m_num_players;
	
	Font m_font;
};

#endif

