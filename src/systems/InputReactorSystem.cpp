#include "InputReactorSystem.h"

#include "core/system.h"
#include "core/coordinator.h"


extern Coordinator gCoordinator;

float speed_factor = 40.0f;

static const int16_t joystick_border = 32600;

void InputReactorSystem::Update(ControllerInput& input)
{
	for (auto const& entity : mEntities)
	{
		auto& inputReactor = gCoordinator.GetComponent<InputReact>(entity);
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& player = gCoordinator.GetComponent<Player>(entity);
		
		switch(inputReactor.actor_type)
		{
			case InputReactorType::NONE:{break;}
			case InputReactorType::PLAYER:
			{
				//do player specific event handling
				//get other player specific component from entity.
				
				for(size_t i = 0; i < input.gamepads_vec.size(); i++)
				{
					//if player number matches game pad
					if(inputReactor.player_num == i + 1)
					{
						//if moved left joystick left
						if(input.gamepads_vec[i].left_x_dir_axis == -1 
							|| input.gamepads_vec[i].left_x_axis < -joystick_border )
						{
							rigidBody.velocity.x = -speed_factor;
						}
						//else if moved left joystick right
						else if( input.gamepads_vec[i].left_x_dir_axis == 1 
								|| input.gamepads_vec[i].left_x_axis > joystick_border )
						{
							rigidBody.velocity.x = speed_factor;
						}
						else
						{
							rigidBody.velocity.x = 0.0f;
						}
						
						//set power requested based on position of right joystick
						//       0
						//    7     1
						//  6		  2
						//    5     3
						//       4
						//if up
						if(input.gamepads_vec[i].right_y_dir_digital == -1)
						{
							if(input.gamepads_vec[i].right_x_dir_digital == -1)
							{
								player.requested_power = 7;
							}
							else if(input.gamepads_vec[i].right_x_dir_digital == 1)
							{
								player.requested_power = 1;
							}
							else
							{
								player.requested_power = 0;
							}
						}
						else if(input.gamepads_vec[i].right_y_dir_digital == 1)
						{
							if(input.gamepads_vec[i].right_x_dir_digital == -1)
							{
								player.requested_power = 5;
							}
							else if(input.gamepads_vec[i].right_x_dir_digital == 1)
							{
								player.requested_power = 3;
							}
							else
							{
								player.requested_power = 4;
							}
						}
						else
						{
							if(input.gamepads_vec[i].right_x_dir_digital == -1)
							{
								player.requested_power = 6;
							}
							else if(input.gamepads_vec[i].right_x_dir_digital == 1)
							{
								player.requested_power = 2;
							}
							else
							{
								player.requested_power = -1;
							}
						}
						
						
						//if jump button pressed
						if( input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_B ||
							input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
						{
							rigidBody.velocity.y = -speed_factor;
						}
						
						//if special power button pressed
						if(input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
						{
							player.powerButtonPressed = true;
						}
						
						if(input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_A)
						{
							player.regularAttackButtonPressed = true;
						}
					}
					
				}
				
				
				break;
			}
			case InputReactorType::CAR:{break;}
			default:{break;}
		}
		
	}
}
