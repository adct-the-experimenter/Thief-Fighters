#include "AttackPowerMechanicSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void AttackPowerMechanicSystem::Init(std::uint8_t num_players)
{
	for(size_t i = 0; i < 8; i++)
	{
		player_attack_boxes_ptrs[i] = nullptr;
		player_health_ptrs[i] = nullptr;
		player_position_ptrs[i] = nullptr;
		player_last_hit_by_ptrs[i] = nullptr;
		player_alive_ptrs[i] = nullptr;
		player_taking_damage_state_ptrs[i] = nullptr;
	}
		
	for (auto const& entity : mEntities)
	{
		auto& player = gCoordinator.GetComponent<Player>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		
		player_health_ptrs[player.player_num - 1] = &player.player_health;
		
		player_attack_boxes_ptrs[player.player_num - 1] = &player.attack_box;
		
		player_position_ptrs[player.player_num - 1] = &transform.position;
		
		player_last_hit_by_ptrs[player.player_num - 1] = &player.last_hit_by_player_num;
		
		player_alive_ptrs[player.player_num - 1] = &player.alive;
		
		player_taking_damage_state_ptrs[player.player_num - 1] = &player.taking_damage;
		
		//set collected power for player
		player.collected_powers[player.current_power] = 1;
	}
	
	m_num_players = num_players;
}

static void RunAttackPowerProcess(std::uint8_t& power)
{
	switch(power)
	{
		case 0:
		{
			//change player attack collision box to match power based on time
			
			//set attack mode in animation
			
			break;
		}
		default:{std::cout << "Calling undefined power!\n"; break;}
	}
}


void AttackPowerMechanicSystem::Update(float& dt)
{
	
	for (auto const& entity : mEntities)
	{
		
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& player = gCoordinator.GetComponent<Player>(entity);
		auto& animation = gCoordinator.GetComponent<Animation>(entity);
		
		if(player.regularAttackButtonPressed && player.alive && !player.taking_damage)
		{
			//if attack box is not active i.e. player is not attacking already
			if(!player.attack_box.active)
			{
				animation.attackMode = 0;
			
				//initialize attack box
				
				player.attack_box.active = true;
				
				//activate collision box
				player.attack_box.collisionBox.x = transform.position.x + 10; 
				player.attack_box.collisionBox.y = transform.position.y + 20;
				player.attack_box.collisionBox.width = 30;
				player.attack_box.collisionBox.height = 30;
				
				player.attack_box.player_num = player.player_num;
				
			}
			
			player.regularAttackButtonPressed = false;
		}
		
		//change and/or activate current power based on input
		else if(player.powerButtonPressed &&
		   player.requested_power < 8 && player.alive && !player.taking_damage)
		{
			std::cout << "Player " << int(player.player_num) << " requested this power: " << int(player.requested_power) << std::endl;
			//check which power player is requesting
			//change to power requested if player has this power
			if( player.requested_power != -1 && player.collected_powers[player.requested_power])
			{
				//activate power, if not actived
				if( !player.powers_activated[player.current_power])
				{
					player.current_power = player.requested_power;
					
					animation.attackMode = player.current_power + 1;
					
					std::cout << "current power of player " << int(player.player_num) << ": " << int(player.current_power) << std::endl;
					player.powers_activated[player.current_power];
				}
				
				
			}
			//else use the current power
			else
			{				
				//activate power if not actived
				if( !player.powers_activated[player.current_power])
				{
					animation.attackMode = player.current_power + 1;
					
					std::cout << "current power of player " << int(player.player_num) << ": " << int(player.current_power) << std::endl;
					player.powers_activated[player.current_power];
				}
			}
			
			//reset requested power
			player.requested_power = -1;
			//reset power button pressed
			player.powerButtonPressed = false;
			
		}
		
		
		//cooldown for regular attack or special power if activated
		for(size_t i = 0; i < 8; i++)
		{
			//regular attack
			if(player.attack_box.active)
			{
				player.regular_attack_cooldown_timer_val += dt;
				
				//if more than 1 second has passed
				if(player.regular_attack_cooldown_timer_val >= 1)
				{
					//reset attackbox active
					player.attack_box.active = false;
					//reset timer value
					player.regular_attack_cooldown_timer_val = 0;
				}
			}
			
			//special power
			if(player.powers_activated[i])
			{
				player.sp_attack_cooldown_timer_val_array[i] += dt;
				
				//if more than 1 second has passed
				if(player.sp_attack_cooldown_timer_val_array[i] >= 1)
				{
					//disable attack box
					
					//reset attackbox active
					player.attack_box.active = false;
					
					//if more than 2 seconds have passed
					if(player.sp_attack_cooldown_timer_val_array[i] >= 2)
					{
						//reset bitset for power activated if cooldown time has finished
						player.powers_activated[i] = 0;
						//reset cooldown timer value
						player.sp_attack_cooldown_timer_val_array[i] = 0;
					}
				}
				
			}
		}
		
		
		//power steal mechanic here
		
		//if player health is at 0
		//check which player had the last collision detection with this player
		if(player.player_health <= 0 && player.last_hit_by_player_num != 0)
		{
			player.alive = false;
			
			//push power transaction to queue
			
			PowerTransferTransaction pT;
			pT.receivingPlayer = player.last_hit_by_player_num;
			pT.powerTransfered = player.collected_powers;
			
			power_transfer_transaction_queue.push(pT);
			
			//set last player hit number to zero
			player.last_hit_by_player_num = 0;
		}
		
		
	}
	
	//collision detection between attack box and player
	AttackPowerMechanicSystem::CollisionDetectionBetweenPlayers();
	
	//perform power transfer based on info from queue
	while(!power_transfer_transaction_queue.empty())
	{
		for (auto const& entity : mEntities)
		{
			auto& player = gCoordinator.GetComponent<Player>(entity);
			
			//if player is the one that must receive power
			if(power_transfer_transaction_queue.front().receivingPlayer == player.player_num)
			{
				//OR player collected power bits with the transfered power bits
				player.collected_powers |= power_transfer_transaction_queue.front().powerTransfered;
				power_transfer_transaction_queue.pop();
				
			}
		}
	}
	
}

static bool PlayerCollisionWithRectangleDetected(Rectangle& rect,
						   float& obj_x, float& obj_y, std::uint32_t& obj_width, std::uint32_t& obj_height)
{
	//assuming object has width and height of 30 and it is centered
	
	float objLeftX = obj_x;
	float objRightX = obj_x + obj_width;
	float objTopY = obj_y;
	float objBottomY = obj_y + obj_height;
	
	std::uint32_t rectLeftX = rect.x;
	std::uint32_t rectRightX = rect.x + rect.width;
	std::uint32_t rectTopY = rect.y;
	std::uint32_t rectBottomY = rect.y + rect.height;
	
	//for collision to be true, all conditions must be true. AABB square collsion detection, all
	//The left edge x-position of [A] must be less than the right edge x-position of [B].
    //The right edge x-position of [A] must be greater than the left edge x-position of [B].
    //The top edge y-position of [A] must be less than the bottom edge y-position of [B].
    //The bottom edge y-position of [A] must be greater than the top edge y-position of [B].
    
    if(objBottomY <= rectTopY)
	{
		return false;
	}
	
	if(objTopY >= rectBottomY)
	{
		return false;
	}
    
    if(objRightX <= rectLeftX)
	{
		return false;
	}
	
	if(objLeftX >= rectRightX)
	{
		return false;
	}
	
	return true;
}

bool AttackPowerMechanicSystem::AreBothPlayersAlive(int& player_a_num, int& player_b_num)
{
	if(*player_alive_ptrs[player_a_num - 1] && *player_alive_ptrs[player_b_num - 1])
	{
		return true;
	}
	
	return false;
}

AttackEvent AttackPowerMechanicSystem::CheckCollisionBetween2Players(int& player_a_num, int& player_b_num)
{
	AttackEvent attack_event;
	attack_event.attack = false;
	attack_event.player_num_victim = 0;
	attack_event.player_num_attacker = 0;
	
	if(!AttackPowerMechanicSystem::AreBothPlayersAlive(player_a_num,player_b_num))
	{
		return attack_event;
	}
	
	AttackBox* playerA_attackbox_ptr = player_attack_boxes_ptrs[player_a_num - 1];
	AttackBox* playerB_attackbox_ptr = player_attack_boxes_ptrs[player_b_num - 1];
	
	Vector2* playerA_position_ptr = player_position_ptrs[player_a_num - 1];
	Vector2* playerB_position_ptr = player_position_ptrs[player_b_num - 1];
	
	//player width and height
	std::uint32_t width = 30;
	std::uint32_t height = 80;

	//if this player A attack box is active, player B attack box inactive
	if( playerA_attackbox_ptr->active && !playerB_attackbox_ptr->active)
	{
		if(PlayerCollisionWithRectangleDetected(playerA_attackbox_ptr->collisionBox,
												playerB_position_ptr->x, playerB_position_ptr->y, width,height)
		   )
		{
			
			
			attack_event.attack = true;
			attack_event.player_num_victim = player_b_num;
			attack_event.player_num_attacker = player_a_num;
			
		}
	}
	//if player B attack box is active, player A attack box inactive
	else if(playerB_attackbox_ptr->active && !playerA_attackbox_ptr->active)
	{
		if(PlayerCollisionWithRectangleDetected(playerB_attackbox_ptr->collisionBox,
												playerA_position_ptr->x, playerA_position_ptr->y, width,height)
		   )
		{
			attack_event.attack = true;
			attack_event.player_num_victim = player_a_num;
			attack_event.player_num_attacker = player_b_num;
			
		}
	}
	
	return attack_event;
}



void AttackPowerMechanicSystem::CollisionDetectionBetweenPlayers()
{
	int player_a_num = 0;
	int player_b_num = 0;
	
	//if there is more than 1 player
	//check player 1 and player 2 interaction
	if( m_num_players > 1)
	{
		AttackEvent attack_event;
		player_a_num = 1;
		player_b_num = 2;
		attack_event = AttackPowerMechanicSystem::CheckCollisionBetween2Players(player_a_num, player_b_num);
		
		//if attack happened, and player is not already in state of taking damage
		if(attack_event.attack && !*player_taking_damage_state_ptrs[attack_event.player_num_victim - 1] )
		{
			//std::cout << "Player " << attack_event.player_num_attacker << "took away 3 HP from player " << attack_event.player_num_victim << std::endl;
			
			//decrease health of victim player
			*player_health_ptrs[attack_event.player_num_victim - 1] -= 3;
			//set last hit by player variable for victim player
			*player_last_hit_by_ptrs[attack_event.player_num_victim - 1] = attack_event.player_num_attacker;
			
			*player_taking_damage_state_ptrs[attack_event.player_num_victim - 1] = true;
			
			//std::cout << "Player " << attack_event.player_num_attacker << " health: " << *player_health_ptrs[attack_event.player_num_victim - 1] << std::endl;
		}
		else
		{
			*player_taking_damage_state_ptrs[player_a_num - 1] = false;
			*player_taking_damage_state_ptrs[player_b_num - 1] = false;
		}
	}
	
	//if there are more than 2 players
	//check player 1 and player 3, player 2 and player 3 interactions
	if( m_num_players > 2 )
	{
		AttackEvent attack_event;
		
		//check player 1 and 3 interaction
		
		player_a_num = 1;
		player_b_num = 3;
		attack_event = AttackPowerMechanicSystem::CheckCollisionBetween2Players(player_a_num, player_b_num);
		
		//if attack happened
		if(attack_event.attack && !*player_taking_damage_state_ptrs[attack_event.player_num_victim - 1] )
		{
			//decrease health of victim player
			*player_health_ptrs[attack_event.player_num_victim - 1] -= 10;
			//set last hit by player variable for victim player
			*player_last_hit_by_ptrs[attack_event.player_num_victim - 1] = attack_event.player_num_attacker;
			
		}
		
		//check player 2 and player 3 interaction
		
		player_a_num = 2;
		player_b_num = 3;
		attack_event = AttackPowerMechanicSystem::CheckCollisionBetween2Players(player_a_num, player_b_num);
		
		//if attack happened
		if(attack_event.attack)
		{
			//decrease health of victim player
			*player_health_ptrs[attack_event.player_num_victim - 1] -= 10;
			//set last hit by player variable for victim player
			*player_last_hit_by_ptrs[attack_event.player_num_victim - 1] = attack_event.player_num_attacker;
			
		}
		
	}
	
	//if there are more than 3 players
	//check player 1 and player 4, player 2 and player 4, player 3 and player 4 interactions
	if( m_num_players > 3)
	{
		AttackEvent attack_event;
		
		//check player 1 and player 4 interaction
		player_a_num = 1;
		player_b_num = 4;
		attack_event = AttackPowerMechanicSystem::CheckCollisionBetween2Players(player_a_num, player_b_num);
		
		//if attack happened
		if(attack_event.attack)
		{
			//decrease health of victim player
			*player_health_ptrs[attack_event.player_num_victim - 1] -= 10;
			//set last hit by player variable for victim player
			*player_last_hit_by_ptrs[attack_event.player_num_victim - 1] = attack_event.player_num_attacker;
			
		}
		
		//check player 2 and player 4 interaction
		player_a_num = 2;
		player_b_num = 4;
		attack_event = AttackPowerMechanicSystem::CheckCollisionBetween2Players(player_a_num, player_b_num);
		
		//if attack happened
		if(attack_event.attack)
		{
			//decrease health of victim player
			*player_health_ptrs[attack_event.player_num_victim - 1] -= 10;
			//set last hit by player variable for victim player
			*player_last_hit_by_ptrs[attack_event.player_num_victim - 1] = attack_event.player_num_attacker;
			
		}
		
		//check player 3 and player 4 interaction
		player_a_num = 3;
		player_b_num = 4;
		attack_event = AttackPowerMechanicSystem::CheckCollisionBetween2Players(player_a_num, player_b_num);
		
		//if attack happened
		if(attack_event.attack)
		{
			//decrease health of victim player
			*player_health_ptrs[attack_event.player_num_victim - 1] -= 10;
			//set last hit by player variable for victim player
			*player_last_hit_by_ptrs[attack_event.player_num_victim - 1] = attack_event.player_num_attacker;
			
		}
		
	}
}
