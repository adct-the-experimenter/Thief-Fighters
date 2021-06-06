#include "stage_selector.h"

#include "core/system.h"
#include "core/coordinator.h"

#include "misc/level_maps.h" //for texture

extern Coordinator gCoordinator;

StageSelector::StageSelector()
{
	m_stage_selected = 0;
	
	stage_confirmation = false;
	
	move_next_state = false;
	
	m_num_stages = 3;
		
}

StageSelector::~StageSelector()
{
	
}

void StageSelector::Init(std::uint8_t num_stages)
{
	
	m_num_stages = num_stages;
	
	stage_boxes.resize(m_num_stages);
	
	//initialize name slot text box locations for each character
	for(size_t it = 0; it < stage_boxes.size(); it++)
	{
		stage_boxes[it].name_slot.textBox = {static_cast<float>( 100*(it+1) ),20,50,20};
	}
}

void StageSelector::handle_input(ControllerInput& controller_input, KeyboardInput& key_input)
{
	StageSelector::handle_controller_input(controller_input);
	StageSelector::handle_keyboard_input(key_input);
}

const int16_t joystick_border = 32600;

void StageSelector::handle_controller_input(ControllerInput& input)
{
	//number of character boxes and number of players should be the same
	
	//for each controller
	for(size_t i = 0; i < input.gamepads_vec.size();i++)
	{
		//if joystick moved up, go up a slot
		if(input.gamepads_vec[i].left_y_dir_digital == -1)
		{
			if(m_stage_selected > 4)
			{
				m_stage_selected = m_stage_selected - 4;
			}
		}
		//else if joystick moved down, go down a slot
		else if(input.gamepads_vec[i].left_y_dir_digital == 1)
		{
			if(m_stage_selected < m_num_stages - 4)
			{
				m_stage_selected = m_stage_selected + 4;
			}
		}
			
		//if joystick moved left, go left on color choice
		if(input.gamepads_vec[i].left_x_dir_digital == -1)
		{
			if(m_stage_selected > 0){m_stage_selected--;}
		}
		//if joystick moved right, go right on color choice
		else if(input.gamepads_vec[i].left_x_dir_digital == 1)
		{
			if(m_stage_selected < m_num_stages - 1){m_stage_selected++;}
		}
		
		//if a button pressed, turn confirm bool on
		if(input.gamepads_vec[i].button_up_released == SDL_CONTROLLER_BUTTON_A)
		{
			stage_confirmation = true;
		}
	}
	
}

void StageSelector::handle_keyboard_input(KeyboardInput& input)
{
	
}

void StageSelector::logic()
{
	int width = 30;
	
	for(int i = 0; i < stage_boxes.size(); i++)
	{
		
		//if selection confirmed
		if(stage_confirmation)
		{
			move_next_state = true;
			break;
		}
		
	}
	
}

void StageSelector::render()
{
	
	//render body part slots
	for(size_t i = 0; i < stage_boxes.size(); i++)
	{
		Vector2 position = { static_cast<float>( 100*(i+1) + i*50 ),0};
		
		Color renderColor = WHITE;
		if(m_stage_selected == i){renderColor = (Color){100,100,20,255};}
		
		//if selection is not confirmed
		if(!stage_confirmation)
		{
			//draw textures of each slot
			int row = trunc(i / 4);
			int col = i - ( trunc( i / 4 ) * 4);
			
			float x = 20 + col*150;
			float y = 50 + row*20;
						
			//render texture of profile
			DrawTexture(stage_profile_textures[i],x, y,renderColor);
			
			//render text box for typing slot
			
		}
		
	}
	
}

void StageSelector::sound()
{
	//play some character select music
}

bool StageSelector::MoveToNextStateBool(){return move_next_state;}

std::uint8_t StageSelector::StageSelected(){return m_stage_selected;}
