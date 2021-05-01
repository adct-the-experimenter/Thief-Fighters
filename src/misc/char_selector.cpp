#include "char_selector.h"

#include "core/system.h"
#include "core/coordinator.h"

#include "misc/media.h" //for texture
#include <array>

extern Coordinator gCoordinator;

static std::array <std::string,3> special_power_choices = {"Long Arms","Dash","Shield"};

CharacterSelector::CharacterSelector()
{
	std::array <Color,8> t_colors = {WHITE,BLUE,BEIGE,GRAY,BROWN,RED,GOLD,LIGHTGRAY};
	colors = t_colors;
	
	move_next_state = false;
	
	m_num_fighters = 2;
	m_num_special_powers = 3;
}

CharacterSelector::~CharacterSelector()
{
	
}

void CharacterSelector::Init(std::vector <Entity> *entities_vec_ptr, std::uint8_t num_players)
{
	//initialize vector holding pointers to player entitites
	//assuming first entities in entire entity vector are player entities.
	
	player_entities_vec.resize(num_players);
	
	for(size_t i = 0; i < player_entities_vec.size(); i++)
	{
		player_entities_vec[i] = &entities_vec_ptr->at(i);
	}
	
	fighter_boxes.resize(num_players);
	
	char_confirmations.resize(num_players);
	
	//set up fighter box positions and internals
	for(size_t i = 0; i < fighter_boxes.size(); i++)
	{
		//for top side
		if(i < 3)
		{
			fighter_boxes[i].background_box.x = i*150 + 10;
			fighter_boxes[i].background_box.y = 50;
			fighter_boxes[i].background_box.width = 150;
			fighter_boxes[i].background_box.height = 225;
			
			fighter_boxes[i].player_num_string = "Player " + std::to_string(i + 1);
			
			fighter_boxes[i].player_num_rect.x = i*150 + 50;
			fighter_boxes[i].player_num_rect.y = 75;
			fighter_boxes[i].player_num_rect.width = 80;
			fighter_boxes[i].player_num_rect.height = 25;
			
			fighter_boxes[i].char_slot_rect.x = i*150 + 50;
			fighter_boxes[i].char_slot_rect.y = 100;
			fighter_boxes[i].char_slot_rect.width = 80;
			fighter_boxes[i].char_slot_rect.height = 25;
			
			fighter_boxes[i].special_power_slot_rect.x = i*150 + 50;
			fighter_boxes[i].special_power_slot_rect.y = 150;
			fighter_boxes[i].special_power_slot_rect.width = 80;
			fighter_boxes[i].special_power_slot_rect.height = 25;
			
			fighter_boxes[i].confirm_selection_rect.x = i*150 + 50;
			fighter_boxes[i].confirm_selection_rect.y = 200;
			fighter_boxes[i].confirm_selection_rect.width = 80;
			fighter_boxes[i].confirm_selection_rect.height = 25;
			
		}
		//for bottom side
		else
		{
			
		}
		
	}
	
	
}

void CharacterSelector::handle_input(ControllerInput& controller_input, KeyboardInput& key_input)
{
	CharacterSelector::handle_controller_input(controller_input);
	CharacterSelector::handle_keyboard_input(key_input);
}

static const int16_t joystick_border = 32600;

void CharacterSelector::handle_controller_input(ControllerInput& input)
{
	//number of character boxes and number of players should be the same
	
	//for each controller
	for(size_t i = 0; i < input.gamepads_vec.size();i++)
	{
		//if joystick moved up, go up a slot
		if(input.gamepads_vec[i].left_y_dir_digital == -1)
		{
			if(fighter_boxes[i].current_slot > 0){fighter_boxes[i].current_slot--;}
			
		}
		//else if joystick moved down, go down a slot
		else if(input.gamepads_vec[i].left_y_dir_digital == 1)
		{
			if(fighter_boxes[i].current_slot < 2){fighter_boxes[i].current_slot++;}
		}
			
		//if joystick moved left, go left on color choice
		if(input.gamepads_vec[i].left_x_dir_digital == -1)
		{
			//if on special power slot
			if(fighter_boxes[i].current_slot == 1)
			{
				if(fighter_boxes[i].special_power_choice > 0){fighter_boxes[i].special_power_choice--;}
			}
		}
		//if joystick moved right, go right on color choice
		else if(input.gamepads_vec[i].left_x_dir_digital == 1)
		{
			//if on special power slot
			if(fighter_boxes[i].current_slot == 1)
			{
				if(fighter_boxes[i].special_power_choice < m_num_special_powers - 1){fighter_boxes[i].special_power_choice++;}
			}
		}
		
		//if a button pressed, turn confirm bool on
		if(input.gamepads_vec[i].button == SDL_CONTROLLER_BUTTON_A)
		{
			//if current slot is confirm slot
			if(fighter_boxes[i].current_slot == 2)
			{
				fighter_boxes[i].confirm_selection = true;
			}
		}
	}
	
}

void CharacterSelector::handle_keyboard_input(KeyboardInput& input)
{
	
}

Vector2 player1_start = {1.0f,2.0f};
Vector2 player2_start = {-1.0f,2.0f};
Vector2 player3_start = {-1.0f,2.0f};
Vector2 player4_start = {-1.0f,2.0f};

void CharacterSelector::logic()
{
	
	for(size_t i = 0; i < fighter_boxes.size(); i++)
	{  	
		
		//if selection confirmed
		if(fighter_boxes[i].confirm_selection)
		{
			//create new render components for player entity, and set player info based on choices.
			//if not already created
			if(!char_confirmations[i])
			{
				char_confirmations[i] = true;
				
				//add render component 
				
				gCoordinator.AddComponent(
								*player_entities_vec[i],
								RenderModelComponent {
									.texture_ptr = &base_fighter_texture,
									.frame_rect = (Rectangle){0,0,30,80} ,
									.render = true
								}
							);
				
				
				//add player info component
				//set special power according to choice
				
				std::bitset <8> collected_powers;
				collected_powers[fighter_boxes[i].special_power_choice] = 1;
				
				gCoordinator.AddComponent(
								*player_entities_vec[i],
								Player {
									.player_num = i + 1,
									.player_health = 30,
									.alive = true,
									.current_power = fighter_boxes[i].special_power_choice,
									.taking_damage = false
									
								}
							);
														
				//add input react component
				InputReact react;
				react.actor_type = InputReactorType::PLAYER;
				react.reactToInput = true;
				react.player_num = i + 1;
								 
				gCoordinator.AddComponent(
								*player_entities_vec[i],
								react
								);
							
				//add transform
				Vector2 initP; 
				if(i == 0){ initP = player1_start;}
				else if(i == 1){ initP = player2_start;}
				else if(i == 2){ initP = player3_start;}
				else if(i == 3){ initP = player4_start;}
				
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							Transform2D{
								.position = initP
							}
						);
				
				//add rigid body
				Vector2 initV = {0.0f,0.0f};
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							RigidBody2D{
								.velocity = initV
							}
						);
						
				//add gravity component for later use
				Vector2 grav = {0.0f,20.0f};
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							Gravity2D{
								.force = grav 
							}
						);
						
				//add physics type
				PhysicsType pType = PhysicsType::PLATFORMER;
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							PhysicsTypeComponent{
								.phy_type = pType 
							}
						);
						

				//add colision box
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							CollisionBox{
								.width = (std::uint32_t){30},
								.height = (std::uint32_t){80}
							}
						);
				
				//add animation component
				
				AnimatedActorType type = AnimatedActorType::PLAYER;
				Animation anim_component = {};
				anim_component.anim_actor_type = type;
				gCoordinator.AddComponent(
							*player_entities_vec.at(i),
							anim_component
						);
			}
			
		}
		
	}
	
	//check if all players confirmed character creations
	bool all_confirmed = true;
	for(size_t i = 0; i < char_confirmations.size(); i++)
	{
		all_confirmed = char_confirmations[i];
		if(!all_confirmed){break;}
	}
	
	if(all_confirmed)
	{
		move_next_state = true;
	}
	
}

void CharacterSelector::render()
{
	
	//render body part slots
	for(size_t i = 0; i < fighter_boxes.size(); i++)
	{
		
		//if selection is not confirmed
		if(!fighter_boxes[i].confirm_selection)
		{
			//background box
			DrawRectangle(fighter_boxes[i].background_box.x, 
							fighter_boxes[i].background_box.y, 
							fighter_boxes[i].background_box.width, 
							fighter_boxes[i].background_box.height, GRAY);
			
			DrawText(fighter_boxes[i].player_num_string.c_str(),
					fighter_boxes[i].player_num_rect.x,
					fighter_boxes[i].player_num_rect.y,
					14,BLACK);
			
			Color char_text_color;
			if(fighter_boxes[i].current_slot == 0){char_text_color = YELLOW;}
			else{char_text_color = BLACK;}
			
			//character fighter
			DrawText("Randy", 
					fighter_boxes[i].char_slot_rect.x, fighter_boxes[i].char_slot_rect.y, 
					14, char_text_color);
			
			//draw texture of character choice
			//DrawTexture(char_select_texture, 200, 100, WHITE);
			
			Color special_power_text_color;
			if(fighter_boxes[i].current_slot == 1){special_power_text_color = YELLOW;}
			else{special_power_text_color = BLACK;}
			
			//draw text for special power
			DrawText(special_power_choices[fighter_boxes[i].special_power_choice].c_str(), 
					fighter_boxes[i].special_power_slot_rect.x, fighter_boxes[i].special_power_slot_rect.y
					, 14, special_power_text_color);
			
			Color confirm_text_color;
			if(fighter_boxes[i].current_slot == 2){confirm_text_color = YELLOW;}
			else{confirm_text_color = BLACK;}
			
			DrawText("Confirm", 
					fighter_boxes[i].confirm_selection_rect.x, fighter_boxes[i].confirm_selection_rect.y
					, 14, confirm_text_color);
		}
		
	}
	
}

void CharacterSelector::sound()
{
	//play some character select music
}

bool CharacterSelector::MoveToNextStateBool(){return move_next_state;}
