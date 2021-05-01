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
			gGameController = SDL_GameControllerOpen( 0 );
			if( gGameController == NULL )
			{
				printf( "Warning: Unable to open game controller 1! SDL Error: %s\n", SDL_GetError() );
			}
			else
			{
				std::cout << "Initialized gamepad 1\n";
			}
			
		}
		
		//if there is a second player
		if(m_num_players > 1)
		{
			gGameController2 = SDL_GameControllerOpen( 1 );
			if( gGameController2 == NULL )
			{
				printf( "Warning: Unable to open game controller 2! SDL Error: %s\n", SDL_GetError() );
			}
			else{std::cout << "Initialized gamepad 2\n";}
		}
		
		
	}
	
	
}

void ControllerInputHandler::Update(ControllerInput* input)
{
	input->Reset();
	ControllerInputHandler::SetGamepadInfo(*input);
}

static const int16_t joystick_border = 32600;

void ControllerInputHandler::SetControllerAxisMotionVariables(ControllerInput::GamepadInfo& gamepad_info, SDL_Event& sdl_event)
{
	if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
	{
		gamepad_info.left_x_axis = sdl_event.jaxis.value;
	}
	else if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
	{
		gamepad_info.left_y_axis = sdl_event.jaxis.value;
	}
	
	if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
	{
		gamepad_info.right_x_axis = sdl_event.jaxis.value;
	}
	else if(sdl_event.jaxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
	{
		gamepad_info.right_y_axis = sdl_event.jaxis.value;
	}
	
	if(gamepad_info.left_x_axis < -1*joystick_border)
	{
		gamepad_info.left_x_dir_digital = -1;
	}
	else if(gamepad_info.left_x_axis > joystick_border)
	{
		gamepad_info.left_x_dir_digital = 1;
	}
	else
	{
		gamepad_info.left_x_dir_digital = 0;
	}
	
	if(gamepad_info.left_y_axis < -1*joystick_border)
	{
		gamepad_info.left_y_dir_digital = -1;
	}
	else if(gamepad_info.left_y_axis > joystick_border)
	{
		gamepad_info.left_y_dir_digital = 1;
	}
	else
	{
		gamepad_info.left_y_dir_digital = 0;
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
				input_info.gamepads_vec[0].button = SDL_GameControllerButton(sdl_event.cbutton.button);
				
				//std::cout << "gamepad 1 button pressed:" << input_info.gamepads_vec[0].button << std::endl;
				
			}
			//gamepad 2
			else if(sdl_event.cbutton.which == 1)
			{
				input_info.gamepads_vec[1].button = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			
			//setting dpad info
			for(size_t i = 0; i < 2; i++)
			{
				
				if(input_info.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_DPAD_LEFT
							  || input_info.gamepads_vec[i].button == 13)
				{
					input_info.gamepads_vec[i].left_x_dir_axis = -1;
					
				}
				else if( input_info.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT
				  || input_info.gamepads_vec[i].button == 14 )
				{
					input_info.gamepads_vec[i].left_x_dir_axis = 1;
					
				}
				
				
				if( input_info.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_DPAD_UP
				  || input_info.gamepads_vec[i].button == 11)
				{
					
					input_info.gamepads_vec[i].left_y_dir_axis = -1;
				}
				else if( input_info.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_DPAD_DOWN
				  || input_info.gamepads_vec[i].button == 12)
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
				input_info.gamepads_vec[0].button = SDL_GameControllerButton(sdl_event.cbutton.button);
				
				//std::cout << "gamepad 1 button released:" << input_info.gamepads_vec[0].button << std::endl;
								
			}
			//gamepad 2
			else if(sdl_event.cbutton.which == 1)
			{
				input_info.gamepads_vec[1].button = SDL_GameControllerButton(sdl_event.cbutton.button);
				
			}
			
			for(size_t i = 0; i < 2; i++)
			{
				//setting dpad info
				if(input_info.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_DPAD_LEFT
							  || input_info.gamepads_vec[i].button == 13)
				{
					input_info.gamepads_vec[i].left_x_dir_axis = 0;
					
				}
				else if( input_info.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT
				  || input_info.gamepads_vec[i].button == 14 )
				{
					input_info.gamepads_vec[i].left_x_dir_axis = 0;
				}
				
				
				if( input_info.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_DPAD_UP
				  || input_info.gamepads_vec[i].button == 11)
				{
					
					input_info.gamepads_vec[i].left_y_dir_axis = 0;
				}
				else if( input_info.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_DPAD_DOWN
				  || input_info.gamepads_vec[i].button == 12)
				{
					input_info.gamepads_vec[i].left_y_dir_axis = 0;
				}
				
			}
		}
		
		if(sdl_event.type == SDL_CONTROLLERAXISMOTION)
		{
			
			//gamepad 1
			
			if(sdl_event.jaxis.which == 0)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[0],sdl_event);
			}
			//gamepad 2
			else if(sdl_event.jaxis.which == 1)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[1],sdl_event);
			}
			//gamepad 3
			else if(sdl_event.jaxis.which == 2)
			{
				ControllerInputHandler::SetControllerAxisMotionVariables(input_info.gamepads_vec[2],sdl_event);
			}
		}
		
    }
	
	
}
