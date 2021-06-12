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

static Color PS_LIGHT_BROWN = (Color){202,186,186,100}; 
static Color PS_LIGHT_GOLD = (Color){225,195,181,100};
static Color PS_LIGHT_ORANGE = (Color){202,198,180,100};
static Color PS_LIGHT_PINK = (Color){202,181,199,100};
static Color PS_LIGHT_WHITE = (Color){255,255,255,100}; 
static Color PS_LIGHT_GREEN = (Color){181,202,186,100};
static Color PS_LIGHT_BLUE = (Color){181,202,201,100};
static Color PS_LIGHT_PURPLE = (Color){194,181,202,100};

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

static std::array <Color,8> player_selection_colors = { PS_LIGHT_BLUE, //LIGHT Brown
											  PS_LIGHT_GOLD, //LIGHT Gold
											  PS_LIGHT_ORANGE, //LIGHT Orange
											  PS_LIGHT_PINK, //LIGHT Pink
											  PS_LIGHT_WHITE, //LIGHT Red
											  PS_LIGHT_GREEN, //LIGHT green
											  PS_LIGHT_BROWN, //LIGHT blue
											  PS_LIGHT_PURPLE //LIGHT purple
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
	
	int background_box_y_row1 = 180;
	
	int background_box_width = 150;
	int background_box_height = 75;
	
	int slot_x_offset = 100;
	int slot_y_offset = 20;
	
	int slot_height = 20;
	int slot_width = 80;
	
	int background_box_y_row2 = 280;
	
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
			
		}
		
	}
	
	//initialize textures for character profiles
	for(size_t i = 0; i < char_profile_wall.char_profiles_textures.size(); i++)
	{
		char_profile_wall.char_profiles_textures[i] = character_profile_textures[i];
		char_profile_wall.player_selection[i] = 0;
		
		float y_factor = 0;
		float x_index_factor = 0;
		
		if(i > 3){y_factor = 60; x_index_factor = 4;}
		
		char_profile_wall.char_profiles_rects[i] = {(i - x_index_factor)*60 + 30, 60 + y_factor, 25,20};
	
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
		if(input.gamepads_vec[i].left_y_dir_digital == -1 && fighter_boxes[i].current_slot == 0)
		{
			
			//move player selection up if not in first row
			if(char_profile_wall.player_selection[i] > 4)
			{
				char_profile_wall.player_selection[i] -= 4;
			}
			
		}
		//else if joystick moved down, go down a slot
		else if(input.gamepads_vec[i].left_y_dir_digital == 1 && fighter_boxes[i].current_slot == 0)
		{
			
			//move player selection down if not in last row
			if(char_profile_wall.player_selection[i] < 4)
			{
				char_profile_wall.player_selection[i] += 4;
			}
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
				//move player selection to left if not on first character
				if(char_profile_wall.player_selection[i] > 0)
				{
					char_profile_wall.player_selection[i]--;
				}
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
				//move player selection to right if not on last character
				if(char_profile_wall.player_selection[i] < 7)
				{
					char_profile_wall.player_selection[i]++;
				}
			}
			
		}
		
		//if a button pressed, turn confirm bool on
		if(input.gamepads_vec[i].button_up_released == SDL_CONTROLLER_BUTTON_A)
		{
			//if current slot is character selection
			if(fighter_boxes[i].current_slot == 0)
			{
				fighter_boxes[i].confirm_char_choice = true;
				fighter_boxes[i].current_slot++;
			}
			
			//if current slot is power choice slot
			else if(fighter_boxes[i].current_slot == 1)
			{
				fighter_boxes[i].confirm_power_choice = true;
				char_confirmations[i] = true;
				fighter_boxes[i].current_slot++;
			}
		}
		
		//if b button pressed, 
		if(input.gamepads_vec[i].button_up_released == SDL_CONTROLLER_BUTTON_B)
		{
						
			//if current slot is power choice slot
			if(fighter_boxes[i].current_slot == 1)
			{
				fighter_boxes[i].confirm_power_choice = false;
				fighter_boxes[i].current_slot--;
			}
			
			//if current slot is final slot
			else if(fighter_boxes[i].current_slot == 2)
			{
				fighter_boxes[i].confirm_power_choice = false;
				char_confirmations[i] = false;
				fighter_boxes[i].current_slot--;
			}
		}
		
		fighter_boxes[i].char_choice = char_profile_wall.player_selection[i];
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
	
	//check if all players confirmed character creations
	bool all_confirmed = true;
	for(size_t i = 0; i < char_confirmations.size(); i++)
	{
		all_confirmed = char_confirmations[i];
		if(!all_confirmed){break;}
	}
	
	//if everyone confirmed, start initializing players
	if(all_confirmed)
	{
		for(size_t i = 0; i < fighter_boxes.size(); i++)
		{  	
			
		
			//create new render components for player entity, and set player info based on choices.
			//if not already created
			
			m_req_char.requested_by_player[i] = character_names[fighter_boxes[i].char_choice];
			m_req_char.char_texture_index_req[i] = fighter_boxes[i].char_choice;
			
			//increment the number of times char choice made in array
			char_chosen_num_times_array[fighter_boxes[i].char_choice] = char_chosen_num_times_array[fighter_boxes[i].char_choice] + 1;
			
			//add render component 
			
			//std::cout << "char chosen num times: " < int(char_chosen_num_times_array[fighter_boxes[i].char_choice]) << std::endl;
			
			RenderModelComponent render_model = {};
			render_model.char_texture_index = fighter_boxes[i].char_choice;
			render_model.frame_rect = (Rectangle){0,0,30,60};
			render_model.tint = player_colors[char_chosen_num_times_array[fighter_boxes[i].char_choice]];
			render_model.render = true;
			
			gCoordinator.AddComponent(
							*player_entities_vec[i],
							render_model
						);
			
			
			//add player info component
			//set special power according to choice
			
			std::bitset <8> collected_powers;
			collected_powers[fighter_boxes[i].special_power_choice] = 1;
			
			Player player_comp = {};
			player_comp.player_num = static_cast<uint8_t>(i + 1),
			player_comp.player_health = 30;
			player_comp.alive = true;
			player_comp.current_power = fighter_boxes[i].special_power_choice;
			player_comp.taking_damage = false;
			player_comp.state = PlayerState::IDLE;
			player_comp.hurt_invincible = false;
								
			gCoordinator.AddComponent(
							*player_entities_vec[i],
							player_comp
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
		
		move_next_state = true;
	}
	
	
}

void CharacterSelector::render()
{
	//render fighter boxes
	for(size_t i = 0; i < fighter_boxes.size(); i++)
	{
		
		//if selection is not confirmed
		if(!fighter_boxes[i].confirm_power_choice)
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
						
		}
		
	}
	
	
	
	//render character profile wall
	for(size_t i = 0; i < char_profile_wall.char_profiles_textures.size(); i++)
	{
		float y_factor = 0;
		float x_index_factor = 0;
		
		if(i > 3){y_factor = 60; x_index_factor = 4;}
		
		
		DrawTextureEx(char_profile_wall.char_profiles_textures[i],
					Vector2{char_profile_wall.char_profiles_rects[i].x,char_profile_wall.char_profiles_rects[i].y},
					0,2,WHITE);
		
	}
	
	for(size_t i = 0; i < player_entities_vec.size(); i++)
	{
		DrawRectangle(char_profile_wall.char_profiles_rects[char_profile_wall.player_selection[i]].x, 
					  char_profile_wall.char_profiles_rects[char_profile_wall.player_selection[i]].y, 
					  char_profile_wall.char_profiles_rects[char_profile_wall.player_selection[i]].width*2, 
					  char_profile_wall.char_profiles_rects[char_profile_wall.player_selection[i]].height*2, 
					  player_selection_colors[i]);
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
