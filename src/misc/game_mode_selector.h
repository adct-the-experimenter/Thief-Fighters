#ifndef GAME_MODE_SELECTOR_H
#define GAME_MODE_SELECTOR_H

#include <cstdint>

#include <array>

#include "raylib.h"
#include "core/ControllerInput.h"
#include "core/KeyboardInput.h"
#include "core/entity.h"

#include <vector>
#include <string>


struct ModeBox
{	
	Rectangle render_rect;
	std::string text;
};

class GameModeSelector
{
public:
	
	GameModeSelector();
	~GameModeSelector();
	
	void Init();
	
	//game loop functions
	
	void handle_input(ControllerInput& controller_input, KeyboardInput& key_input);
	
	void logic();
	
	void render();
	
	void sound();
	
	bool MoveToNextStateBool();
	
	bool MoveToPreviousStateBool();
		
	std::uint8_t GetModeSelected();
	
	void Reset(); 
	
private:
	
	//array to hold mode box
	std::array <ModeBox,2> mode_boxes;
	
	std::uint8_t m_current_mode_select;
	bool m_confirm_selection;
	
	bool move_next_state;
	
	bool move_prev_state;
	
	void handle_controller_input(ControllerInput& controller_input);
	
	Font m_font;
};

#endif
