#include "ControllerInputHandler.h"

#include "raylib.h"
#include <iostream>

#if defined(PLATFORM_RPI)
    #define XBOX360_NAME_ID     "Microsoft X-Box 360 pad"
    #define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#else
    #define XBOX360_NAME_ID     "Xbox 360 Controller"
    #define PS3_NAME_ID         "PLAYSTATION(R)3 Controller"
#endif

void ControllerInputHandler::InitGamepad(SDL_GameController* game_controller_ptr, std::uint8_t num_gamepad)
{
	game_controller_ptr = SDL_GameControllerOpen( num_gamepad );
	if( gGameController == NULL )
	{
		std::cout << "Warning: Unable to open game controller" << num_gamepad;
		printf(" ! SDL Error: %s\n", SDL_GetError() );
	}
	else
	{
		std::cout << "Initialized gamepad " << num_gamepad << "\n";
	}
}

void ControllerInputHandler::Init(std::uint8_t num_players)
{
	
	m_num_players = num_players;
	
	std::string mapping_file = DATADIR_STR + "/gamecontrollerdb.txt";
	//SetGamepadMappings(mapping_file.c_str());
	
	if( SDL_GameControllerAddMappingsFromFile(mapping_file.c_str()) == -1)
	{
		std::cout << "Failed to add gamepad mappings using SDL2!\n";
	}
	else
	{
		std::cout << "Successfully added gamepad mappings using SDL2!\n";
	}
	
	//Check for joysticks
	if( SDL_NumJoysticks() < 1 )
	{
		printf( "Warning: No joysticks connected!\n" );
	}
	else
	{
		//Load gamepad controllers
		
		//first player 
		if(m_num_players > 0)
		{
			ControllerInputHandler::InitGamepad(gGameController, 0);
			
		}
		
		//if there is a second player
		if(m_num_players > 1)
		{
			ControllerInputHandler::InitGamepad(gGameController2, 1);
		}
		
		//if there is a third player
		if(m_num_players > 2)
		{
			ControllerInputHandler::InitGamepad(gGameController3, 2);
		}
		
		//if there is a fourth player
		if(m_num_players > 3)
		{
			ControllerInputHandler::InitGamepad(gGameController4, 3);
		}
		
		//if there is a fifth player
		if(m_num_players > 4)
		{
			ControllerInputHandler::InitGamepad(gGameController5, 4);
		}
		
		//if there is a sixth player
		if(m_num_players > 5)
		{
			ControllerInputHandler::InitGamepad(gGameController6, 5);
		}
		
		//if there is a seventh player
		if(m_num_players > 6)
		{
			ControllerInputHandler::InitGamepad(gGameController7, 6);
		}
		
		//if there is an eighth player
		if(m_num_players > 7)
		{
			ControllerInputHandler::InitGamepad(gGameController8, 7);
		}
	}
	
	
}

void ControllerInputHandler::Update(ControllerInput* input)
{
	input->Reset();
	ControllerInputHandler::SetGamepadInfo(*input);
}

static const int16_t joystick_border = 32767;
static const int16_t joystick_border_analog = 10000;

void ControllerInputHandler::SetControllerAxisMotionVariables(ControllerInput::GamepadInfo& gamepad_info, SDL_Event& sdl_event)
{
	if(sdl_event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
	{
		gamepad_info.left_x_axis = sdl_event.caxis.value;
	}
	else if(sdl_event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
	{
		gamepad_info.left_y_axis = sdl_event.caxis.value;
	}
	
	if(sdl_event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
	{
		gamepad_info.right_x_axis = sdl_event.caxis.value;
	}
	else if(sdl_event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
	{
		gamepad_info.right_y_axis = sdl_event.caxis.value;
	}
	
	//set left digital axis 
		
	if(gamepad_info.left_x_axis <= -1*joystick_border && gamepad_info.left_x_dir_digital == 0)
	{
		gamepad_info.left_x_dir_digital = -1;
	}
	else if(gamepad_info.left_x_axis >= joystick_border  && gamepad_info.left_x_dir_digital == 0)
	{
		gamepad_info.left_x_dir_digital = 1;
	}
	else
	{
		gamepad_info.left_x_dir_digital = 0;
	}
	
	if(gamepad_info.left_y_axis <= -1*joystick_border && gamepad_info.left_y_dir_digital == 0)
	{
		gamepad_info.left_y_dir_digital = -1;
	}
	else if(gamepad_info.left_y_axis >= joystick_border && gamepad_info.left_y_dir_digital == 0)
	{
		gamepad_info.left_y_dir_digital = 1;
	}
	else
	{
		gamepad_info.left_y_dir_digital = 0;
	}
	
	//set right digital axis
	if(gamepad_info.right_x_axis < -1*joystick_border_analog)
	{
		gamepad_info.right_x_dir_digital = -1;
	}
	else if(gamepad_info.right_x_axis > joystick_border_analog)
	{
		gamepad_info.right_x_dir_digital = 1;
	}
	else
	{
		gamepad_info.right_x_dir_digital = 0;
	}
	
	if(gamepad_info.right_y_axis < -1*joystick_border_analog)
	{
		gamepad_info.right_y_dir_digital = -1;
	}
	else if(gamepad_info.right_y_axis > joystick_border_analog)
	{
		gamepad_info.right_y_dir_digital = 1;
	}
	else
	{
		gamepad_info.right_y_dir_digital = 0;
	}
	
}

void ControllerInputHandler::SetGamepadInfo(ControllerInput& input_info)
{
	/*
	 * Raylib isn't working for some gamepads even when its defined in game controller mappings.
	 * Need to wait for update or fix for this issue.
	 * Using SDL2 as a workaround.
	 
	if (IsGamepadAvailable(GAMEPAD_PLAYER1))
    {
		 if (GetGamepadButtonPressed() != -1)
		 { 
			 std::cout << "DETECTED BUTTON: " << GetGamepadButtonPressed();
		 }
	}
	*/
	

	while( SDL_PollEvent(&sdl_event) != 0)
    {
        if(sdl_event.type == SDL_CONTROLLERBUTTONDOWN )
		{
			
			//gamepad 1
			if(sdl_event.cbutton.which == 0)
			{
				input_info.gamepads_vec[0].button_down = SDL_GameControllerButton(sdl_event.cbutton.button);								
			}
			//gamepad 2
			else if(sdl_event.cbutton.which == 1)
			{
				input_info.gamepads_vec[1].button_down = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 3
			else if(sdl_event.cbutton.which == 2)
			{
				input_info.gamepads_vec[2].button_down = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 4
			else if(sdl_event.cbutton.which == 3)
			{
				input_info.gamepads_vec[3].button_down = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 5
			else if(sdl_event.cbutton.which == 4)
			{
				input_info.gamepads_vec[4].button_down = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 6
			else if(sdl_event.cbutton.which == 5)
			{
				input_info.gamepads_vec[5].button_down = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 7
			else if(sdl_event.cbutton.which == 6)
			{
				input_info.gamepads_vec[6].button_down = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 8
			else if(sdl_event.cbutton.which == 7)
			{
				input_info.gamepads_vec[7].button_down = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			
			//setting dpad info
			for(size_t i = 0; i < m_num_players; i++)
			{
				
				if(input_info.gamepads_vec[i].button_down == SDL_CONTROLLER_BUTTON_DPAD_LEFT
							  || input_info.gamepads_vec[i].button_down == 13)
				{
					input_info.gamepads_vec[i].left_x_dir_axis = -1;
					
				}
				else if( input_info.gamepads_vec[i].button_down == SDL_CONTROLLER_BUTTON_DPAD_RIGHT
				  || input_info.gamepads_vec[i].button_down == 14 )
				{
					input_info.gamepads_vec[i].left_x_dir_axis = 1;
					
				}
				
				
				if( input_info.gamepads_vec[i].button_down == SDL_CONTROLLER_BUTTON_DPAD_UP
				  || input_info.gamepads_vec[i].button_down == 11)
				{
					
					input_info.gamepads_vec[i].left_y_dir_axis = -1;
				}
				else if( input_info.gamepads_vec[i].button_down == SDL_CONTROLLER_BUTTON_DPAD_DOWN
				  || input_info.gamepads_vec[i].button_down == 12)
				{
					input_info.gamepads_vec[i].left_y_dir_axis = 1;
				}
				
			}
			
		}
		
		if(sdl_event.type == SDL_CONTROLLERBUTTONUP)
		{
			//gamepad 1
			if(sdl_event.cbutton.which == 0)
			{
				input_info.gamepads_vec[0].button_up_released = SDL_GameControllerButton(sdl_event.cbutton.button);								
			}
			//gamepad 2
			else if(sdl_event.cbutton.which == 1)
			{
				input_info.gamepads_vec[1].button_up_released = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 3
			else if(sdl_event.cbutton.which == 2)
			{
				input_info.gamepads_vec[2].button_up_released = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 4
			else if(sdl_event.cbutton.which == 3)
			{
				input_info.gamepads_vec[3].button_up_released = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 5
			else if(sdl_event.cbutton.which == 4)
			{
				input_info.gamepads_vec[4].button_up_released = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 6
			else if(sdl_event.cbutton.which == 5)
			{
				input_info.gamepads_vec[5].button_up_released = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 7
			else if(sdl_event.cbutton.which == 6)
			{
				input_info.gamepads_vec[6].button_up_released = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			//gamepad 8
			else if(sdl_event.cbutton.which == 7)
			{
				input_info.gamepads_vec[7].button_up_released = SDL_GameControllerButton(sdl_event.cbutton.button);
			}
			
			for(size_t i = 0; i < m_num_players; i++)
			{
				//setting dpad info
				if(input_info.gamepads_vec[i].button_up_released == SDL_CONTROLLER_BUTTON_DPAD_LEFT
							  || input_info.gamepads_vec[i].button_up_released == 13)
				{
					input_info.gamepads_vec[i].left_x_dir_axis = 0;
					
				}
				else if( input_info.gamepads_vec[i].button_up_released == SDL_CONTROLLER_BUTTON_DPAD_RIGHT
				  || input_info.gamepads_vec[i].button_up_released == 14 )
				{
					input_info.gamepads_vec[i].left_x_dir_axis = 0;
				}
				
				
				if( input_info.gamepads_vec[i].button_up_released == SDL_CONTROLLER_BUTTON_DPAD_UP
				  || input_info.gamepads_vec[i].button_up_released == 11)
				{
					
					input_info.gamepads_vec[i].left_y_dir_axis = 0;
				}
				else if( input_info.gamepads_vec[i].button_up_released == SDL_CONTROLLER_BUTTON_DPAD_DOWN
				  || input_info.gamepads_vec[i].button_up_released == 12)
				{
					input_info.gamepads_vec[i].left_y_dir_axis = 0;
				}
				
			}
		}
		
		if(sdl_event.type == SDL_CONTROLLERAXISMOTION)
		{
			
			//gamepad 1
			if(sdl_event.caxis.which == 0)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[0],sdl_event);
			}
			//gamepad 2
			else if(sdl_event.caxis.which == 1)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[1],sdl_event);
			}
			//gamepad 3
			else if(sdl_event.caxis.which == 2)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[2],sdl_event);
			}
			//gamepad 4
			else if(sdl_event.caxis.which == 3)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[3],sdl_event);
			}
			//gamepad 5
			else if(sdl_event.caxis.which == 4)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[4],sdl_event);
			}
			//game pad 6
			else if(sdl_event.caxis.which == 5)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[5],sdl_event);
			}
			//game pad 7
			else if(sdl_event.caxis.which == 6)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[6],sdl_event);
			}
			//game pad 8
			else if(sdl_event.caxis.which == 7)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[7],sdl_event);
			}
		}
		
    }
	
	
}
