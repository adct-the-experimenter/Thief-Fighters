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
		int16_t left_x_axis;
		int16_t left_y_axis;
		
		//right joystick
		int16_t right_x_axis;
		int16_t right_y_axis;
		
		int16_t right_x_dir_digital;
		int16_t right_y_dir_digital;
		
		//joystick direction, gets reset for non-continous movement
		int16_t left_x_dir_axis;
		int16_t left_y_dir_axis;
		
		int16_t left_x_dir_digital;
		int16_t left_y_dir_digital;
		
		//includes buttons and dpad
		//which button is currently pressed down
		SDL_GameControllerButton button_down;
		
		//which button was recently released
		SDL_GameControllerButton button_up_released;
		
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
			gamepads_vec[i].button_down =  SDL_CONTROLLER_BUTTON_INVALID;
			gamepads_vec[i].button_up_released =  SDL_CONTROLLER_BUTTON_INVALID;
			gamepads_vec[i].left_x_dir_digital =  0;
			gamepads_vec[i].left_y_dir_digital =  0;
			gamepads_vec[i].right_x_dir_digital = 0;
			gamepads_vec[i].right_y_dir_digital = 0;
		}
	};
		
	
};

#endif
