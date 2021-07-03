#include "game_mode_selector.h"


static std::string game_mode_choices [2] = {"Versus","Story - Alpha"};

GameModeSelector::GameModeSelector()
{
	m_current_mode_select = 0;
	move_next_state = false;
	m_confirm_selection = false;
}

GameModeSelector::~GameModeSelector()
{
	
}

void GameModeSelector::Init()
{
	m_current_mode_select = 0;
	move_next_state = false;
	m_confirm_selection = false;
	
	//initialize render slot locations
	for(size_t it = 0; it < mode_boxes.size(); it++)
	{
		mode_boxes[it].render_rect = {320 - 100, static_cast<float>( 100 + 30*(it+1) ), 100, 20};
		mode_boxes[it].text = game_mode_choices[it];
	}
	
	m_font = GetFontDefault();
}
	

void GameModeSelector::handle_input(ControllerInput& controller_input, KeyboardInput& key_input)
{
	GameModeSelector::handle_controller_input(controller_input);
}

static const int16_t joystick_border = 32600;
static bool moveBool = false;

void GameModeSelector::handle_controller_input(ControllerInput& input)
{
	//number of character boxes and number of players should be the same
	
	//only first game pad decides number of players
	
	size_t i = 0;
	
	//if joystick moved up, go up a slot
	if(input.gamepads_vec[i].left_y_dir_axis == -1 ||
	   input.gamepads_vec[i].left_y_dir_digital == -1)
	{
		if(m_current_mode_select > 0 && moveBool){m_current_mode_select--;}
		moveBool = false;
	}
	//else if joystick moved down, go down a slot
	else if(input.gamepads_vec[i].left_y_dir_axis == 1 ||
			input.gamepads_vec[i].left_y_dir_digital == 1)
	{
		if(m_current_mode_select < 1 && moveBool){m_current_mode_select++;}
		moveBool = false;
	}
	
	if(input.gamepads_vec[i].left_y_dir_axis == 0 )
	{
		moveBool = true;
	}
	
	
	//if a button pressed, turn confirm bool on
	if(input.gamepads_vec[i].button_up_released == SDL_CONTROLLER_BUTTON_A)
	{
		m_confirm_selection = true;
	}
}

void GameModeSelector::logic()
{
	//if selection confirmed
	if(m_confirm_selection)
	{
		move_next_state = true;
	}
}

void GameModeSelector::render()
{
	for(size_t i = 0; i < mode_boxes.size(); i++)
	{
		
		//draw each slot
		if(i != m_current_mode_select)
		{
			DrawRectangleRec(mode_boxes[i].render_rect,WHITE);
			DrawTextRec(m_font,mode_boxes[i].text.c_str(),mode_boxes[i].render_rect,12,2.0f, true, BLACK);
		}
		else
		{
			DrawRectangleRec(mode_boxes[i].render_rect,YELLOW);
			DrawTextRec(m_font,mode_boxes[i].text.c_str(),mode_boxes[i].render_rect,12,2.0f, true, BLACK);
		}
		
	}
}

void GameModeSelector::sound()
{
	
}

bool GameModeSelector::MoveToNextStateBool(){return move_next_state;}

std::uint8_t GameModeSelector::GetModeSelected(){return m_current_mode_select;}

void GameModeSelector::Reset()
{
	m_current_mode_select = 0;
	move_next_state = false;
	m_confirm_selection = false;
}
