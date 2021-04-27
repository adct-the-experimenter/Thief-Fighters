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
						
						if(input.gamepads_vec[i].left_x_dir_axis == -1 
							|| input.gamepads_vec[i].left_x_axis < -joystick_border )
						{
							rigidBody.velocity.x = -speed_factor;
						}
						else if( input.gamepads_vec[i].left_x_dir_axis == 1 
								|| input.gamepads_vec[i].left_x_axis > joystick_border )
						{
							rigidBody.velocity.x = speed_factor;
						}
						else
						{
							rigidBody.velocity.x = 0.0f;
						}
						
						//if jump button pressed
						if( input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_B ||
							input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
						{
							rigidBody.velocity.y = -speed_factor;
						}
						
						//if special power button pressed
						if(input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
						{
							player.powerButtonPressed = true;
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
