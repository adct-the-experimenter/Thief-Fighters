#include "char_selector.h"

#include "core/system.h"
#include "core/coordinator.h"


#include <array>

extern Coordinator gCoordinator;

static std::array <std::string,5> special_power_choices = {"Sneak","Dash","Shield","Chunks","Big"};


static Color LIGHT_BROWN = (Color){202,186,186,255}; 
static Color LIGHT_GOLD = (Color){225,195,181,255};
static Color LIGHT_ORANGE = (Color){202,198,180,255};
static Color LIGHT_PINK = (Color){202,181,199,255};
static Color LIGHT_WHITE = (Color){255,255,255,255}; 
static Color LIGHT_GREEN = (Color){181,202,186,255};
static Color LIGHT_BLUE = (Color){181,202,201,255};
static Color LIGHT_PURPLE = (Color){194,181,202,255};

static std::array <uint8_t,8> char_chosen_num_times_array;

static std::array <Color,8> player_colors = { LIGHT_BLUE, //LIGHT Brown
											  LIGHT_GOLD, //LIGHT Gold
											  LIGHT_ORANGE, //LIGHT Orange
											  LIGHT_PINK, //LIGHT Pink
											  LIGHT_WHITE, //LIGHT Red
											  LIGHT_GREEN, //LIGHT green
											  LIGHT_BROWN, //LIGHT blue
											  LIGHT_PURPLE //LIGHT purple
											  };

CharacterSelector::CharacterSelector()
{
	
	move_next_state = false;
	
	m_num_fighters = 8;
	m_num_special_powers = special_power_choices.size();
	
	char_chosen_num_times_array.fill(0);
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
	
	int background_box_y_row1 = 50;
	
	int background_box_width = 150;
	int background_box_height = 150;
	
	int slot_x_offset = 100;
	int slot_y_offset = 20;
	
	int slot_height = 20;
	int slot_width = 80;
	
	int background_box_y_row2 = 205;
	
	//set up fighter box positions and internals
	for(size_t i = 0; i < fighter_boxes.size(); i++)
	{
		//for top side
		if(i < 4)
		{
			fighter_boxes[i].background_box.x = i*slot_x_offset + 10;
			fighter_boxes[i].background_box.y = background_box_y_row1;
			fighter_boxes[i].background_box.width = background_box_width;
			fighter_boxes[i].background_box.height = background_box_height;
			
			fighter_boxes[i].player_num_string = "Player " + std::to_string(i + 1);
			
			fighter_boxes[i].player_num_rect.x = i*slot_x_offset + 50;
			fighter_boxes[i].player_num_rect.y = background_box_y_row1 + slot_y_offset;
			fighter_boxes[i].player_num_rect.width = slot_width;
			fighter_boxes[i].player_num_rect.height = slot_height ;
			
			fighter_boxes[i].char_slot_rect.x = i*slot_x_offset + 50;
			fighter_boxes[i].char_slot_rect.y = background_box_y_row1 + 2*slot_y_offset;
			fighter_boxes[i].char_slot_rect.width = slot_width;
			fighter_boxes[i].char_slot_rect.height = slot_height ;
			
			fighter_boxes[i].special_power_slot_rect.x = i*slot_x_offset + 50;
			fighter_boxes[i].special_power_slot_rect.y = background_box_y_row1 + 3*slot_y_offset;
			fighter_boxes[i].special_power_slot_rect.width = slot_width;
			fighter_boxes[i].special_power_slot_rect.height = slot_height;
			
			fighter_boxes[i].confirm_selection_rect.x = i*slot_x_offset + 50;
			fighter_boxes[i].confirm_selection_rect.y = background_box_y_row1 + 4*slot_y_offset;
			fighter_boxes[i].confirm_selection_rect.width = slot_width;
			fighter_boxes[i].confirm_selection_rect.height = slot_height;
			
		}
		//for bottom side
		else
		{
			fighter_boxes[i].background_box.x = (i-4)*slot_x_offset + 10;
			fighter_boxes[i].background_box.y = background_box_y_row2;
			fighter_boxes[i].background_box.width = background_box_width;
			fighter_boxes[i].background_box.height = background_box_height;
			
			fighter_boxes[i].player_num_string = "Player " + std::to_string(i + 1);
			
			fighter_boxes[i].player_num_rect.x = (i-4)*slot_x_offset + 50;
			fighter_boxes[i].player_num_rect.y = background_box_y_row2 + slot_y_offset;
			fighter_boxes[i].player_num_rect.width = slot_width;
			fighter_boxes[i].player_num_rect.height = slot_height ;
			
			fighter_boxes[i].char_slot_rect.x = (i-4)*slot_x_offset + 50;
			fighter_boxes[i].char_slot_rect.y = background_box_y_row2 + 2*slot_y_offset;
			fighter_boxes[i].char_slot_rect.width = slot_width;
			fighter_boxes[i].char_slot_rect.height = slot_height ;
			
			fighter_boxes[i].special_power_slot_rect.x = (i-4)*slot_x_offset + 50;
			fighter_boxes[i].special_power_slot_rect.y = background_box_y_row2 + 3*slot_y_offset;
			fighter_boxes[i].special_power_slot_rect.width = slot_width;
			fighter_boxes[i].special_power_slot_rect.height = slot_height;
			
			fighter_boxes[i].confirm_selection_rect.x = (i-4)*slot_x_offset + 50;
			fighter_boxes[i].confirm_selection_rect.y = background_box_y_row2 + 4*slot_y_offset;
			fighter_boxes[i].confirm_selection_rect.width = slot_width;
			fighter_boxes[i].confirm_selection_rect.height = slot_height;
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
			//if on character slot
			else if(fighter_boxes[i].current_slot == 0)
			{
				if(fighter_boxes[i].char_choice > 0 ){ fighter_boxes[i].char_choice--; }
			}
		}
		//if joystick moved right, go right on color choice
		else if(input.gamepads_vec[i].left_x_dir_digital == 1)
		{
			//if on special power slot
			if(fighter_boxes[i].current_slot == 1)
			{
				if(fighter_boxes[i].special_power_choice < m_num_special_powers - 1){ fighter_boxes[i].special_power_choice++; }
			}
			//if on character slot
			else if(fighter_boxes[i].current_slot == 0)
			{
				if(fighter_boxes[i].char_choice < m_num_fighters - 1 ){ fighter_boxes[i].char_choice++; }
			}
		}
		
		//if a button pressed, turn confirm bool on
		if(input.gamepads_vec[i].button_up_released == SDL_CONTROLLER_BUTTON_A)
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
				m_req_char.requested_by_player[i] = character_names[fighter_boxes[i].char_choice];
				m_req_char.char_texture_index_req[i] = fighter_boxes[i].char_choice;
				
				//increment the number of times char choice made in array
				char_chosen_num_times_array[fighter_boxes[i].char_choice] = char_chosen_num_times_array[fighter_boxes[i].char_choice] + 1;
				
				//add render component 
				
				std::cout << "char chosen num times: " << int(char_chosen_num_times_array[fighter_boxes[i].char_choice]) << std::endl;
				gCoordinator.AddComponent(
								*player_entities_vec[i],
								RenderModelComponent {
									.char_texture_index = fighter_boxes[i].char_choice,
									.frame_rect = (Rectangle){0,0,30,60} ,
									.tint = player_colors[char_chosen_num_times_array[fighter_boxes[i].char_choice]],
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
									.taking_damage = false,
									.state = PlayerState::IDLE,
									.hurt_invincible = false
									
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
								.height = (std::uint32_t){60}
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
					14,player_colors[i]);
			
			Color char_text_color;
			if(fighter_boxes[i].current_slot == 0){char_text_color = YELLOW;}
			else{char_text_color = BLACK;}
			
			//character fighter
			DrawText(character_names[fighter_boxes[i].char_choice].c_str(), 
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

RequestedCharacters& CharacterSelector::GetRequestedCharacters(){return m_req_char;}

void CharacterSelector::Reset()
{
	fighter_boxes.clear();
	fighter_boxes.resize(0);
	
	char_confirmations.clear();
	char_confirmations.resize(0);
	
	move_next_state = false;
	char_chosen_num_times_array.fill(0);
}
