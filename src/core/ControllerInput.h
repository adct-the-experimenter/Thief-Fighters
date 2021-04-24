#ifndef CONTROLLER_INPUT_H
#define CONTROLLER_INPUT_H

#include "misc/globalvariables.h"
#include <cstdint>
#include <vector>

/*

SDL Controller button enums

SDL_CONTROLLER_BUTTON_INVALID
SDL_CONTROLLER_BUTTON_A
SDL_CONTROLLER_BUTTON_B
SDL_CONTROLLER_BUTTON_X
SDL_CONTROLLER_BUTTON_Y
SDL_CONTROLLER_BUTTON_BACK
SDL_CONTROLLER_BUTTON_GUIDE
SDL_CONTROLLER_BUTTON_START
SDL_CONTROLLER_BUTTON_LEFTSTICK
SDL_CONTROLLER_BUTTON_RIGHTSTICK
SDL_CONTROLLER_BUTTON_LEFTSHOULDER
SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
SDL_CONTROLLER_BUTTON_DPAD_UP
SDL_CONTROLLER_BUTTON_DPAD_DOWN
SDL_CONTROLLER_BUTTON_DPAD_LEFT
SDL_CONTROLLER_BUTTON_DPAD_RIGHT
SDL_CONTROLLER_BUTTON_MAX 

*/
										
class ControllerInput
{
public:
	
												
	struct GamepadInfo {
		//left joystick 
		int16_t x_axis;
		int16_t y_axis;
		
		//joystick direction, gets reset for non-continous movement
		int16_t x_dir_axis;
		int16_t y_dir_axis;
		
		//includes buttons and dpad
		SDL_GameControllerButton button;
		//0 is null. 1 is pressed, 2 is released.
		int button_state = 0;
	};
	
	std::vector <GamepadInfo> gamepads_vec;
	
	void Init(std::uint8_t num_players)
	{
		gamepads_vec.clear();
		gamepads_vec.resize(num_players);
	};
	
	void Reset()
	{
		for(size_t i = 0; i < gamepads_vec.size(); i++)
		{
			gamepads_vec[i].button =  SDL_CONTROLLER_BUTTON_INVALID;
		}
	};
};

#endif
