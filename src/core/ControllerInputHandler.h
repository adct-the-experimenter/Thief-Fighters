#ifndef CONTROLLER_INPUT_HANDLER_H
#define CONTROLLER_INPUT_HANDLER_H

#include "core/ControllerInput.h"
#include "misc/globalvariables.h" //for SDL2 gamepad and data dir path string

class ControllerInputHandler
{
public:
	
	void Init(std::uint8_t num_players);
	
	void Update(ControllerInput* input);
	
private:
	
	SDL_Event sdl_event; //polls events
	
	void SetGamepadInfo(ControllerInput& input_info);
	
	std::uint8_t m_num_players;
	
	//game pads
	SDL_GameController* gGameController;
	SDL_GameController* gGameController2;
	SDL_GameController* gGameController3;
	SDL_GameController* gGameController4;
	SDL_GameController* gGameController5;
	SDL_GameController* gGameController6;
	SDL_GameController* gGameController7;
	SDL_GameController* gGameController8;
	
	void InitGamepad(SDL_GameController* game_controller_ptr, std::uint8_t num_gamepad);
	
	void SetControllerAxisMotionVariables(ControllerInput::GamepadInfo& gamepad_info,SDL_Event& sdl_event);
};

#endif
