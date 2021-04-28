#include "AttackPowerMechanicSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void AttackPowerMechanicSystem::Init(std::uint8_t num_players)
{
	player_attack_boxes_ptrs[0] = nullptr;
	player_attack_boxes_ptrs[1] = nullptr;
	player_attack_boxes_ptrs[2] = nullptr;
	player_attack_boxes_ptrs[3] = nullptr;
	player_attack_boxes_ptrs[4] = nullptr;
	player_attack_boxes_ptrs[5] = nullptr;
	player_attack_boxes_ptrs[6] = nullptr;
	player_attack_boxes_ptrs[7] = nullptr;
	current_attack_box_index = 0;
	
	player_health_ptrs[0] = nullptr;
	player_health_ptrs[1] = nullptr;
	player_health_ptrs[2] = nullptr;
	player_health_ptrs[3] = nullptr;
	player_health_ptrs[4] = nullptr;
	player_health_ptrs[5] = nullptr;
	player_health_ptrs[6] = nullptr;
	player_health_ptrs[7] = nullptr;
	
	for (auto const& entity : mEntities)
	{
		auto& player = gCoordinator.GetComponent<Player>(entity);
		
		player_health_ptrs[player.player_num - 1] = &player.player_health;
		
		player_attack_boxes_ptrs[player.player_num - 1] = &player.attack_box;
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
		
		if(player.regularAttackButtonPressed)
		{
			animation.attackMode = 0;
			
			//initialize attack box and put it in array
			
			player.attack_box.active = true;
			
			//activate collision box
			player.attack_box.collisionBox.x = transform.position.x + 10; 
			player.attack_box.collisionBox.y = transform.position.y + 30;
			player.attack_box.collisionBox.width = 30;
			player.attack_box.collisionBox.height = 30;
			
			player.attack_box.player_num = player.player_num;
			
			player.regularAttackButtonPressed = false;
			
		}
		
		//change and/or activate current power based on input
		else if(player.powerButtonPressed && player.requested_power != -1 &&
		   player.requested_power < 8)
		{
			//check which power player is requesting
			//change to power requested if player has this power
			if(player.collected_powers[player.requested_power])
			{
				player.current_power = player.requested_power;
				
				//set power activated bit, if not actived
				if( !player.powers_activated[player.current_power])
				{
					player.powers_activated[player.current_power];
				}
			}
			
			
		}
		
		
		//cooldown for power if activated
		for(size_t i = 0; i < 8; i++)
		{
			if(player.powers_activated[i])
			{
				player.cooldown_timer_val_array[i] += dt;
				
				//if more than 2 seconds has passed
				if(player.cooldown_timer_val_array[i] >= 2)
				{
					//reset bitset for power activated if cooldown time has finished
					player.powers_activated[i] = 0;
					//reset cooldown timer value
					player.cooldown_timer_val_array[i] = 0;
				}
			}
		}
		
		
		//reset requested power
		player.requested_power = -1;
		//reset power button pressed
		player.powerButtonPressed = false;
		
		//power steal mechanic here
		
		//if player health is at 0
		//check which player had the last collision detection with this player
		if(player.player_health == 0 && player.last_hit_by_player_num != 0)
		{
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

static bool CollisionWithPlatformDetected(Rectangle& platform,
						   float& obj_x, float& obj_y, std::uint32_t obj_width, std::uint32_t& obj_height)
{
	//assuming object has width and height of 30 and it is centered
	
	float objLeftX = obj_x;
	float objRightX = obj_x + obj_width;
	float objTopY = obj_y;
	float objBottomY = obj_y + obj_height;
	
	std::uint32_t rectLeftX = platform.x;
	std::uint32_t rectRightX = platform.x + platform.width;
	std::uint32_t rectTopY = platform.y;
	std::uint32_t rectBottomY = platform.y + platform.height;
	
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

void AttackPowerMechanicSystem::CollisionDetectionBetweenPlayers()
{
	
	//if there is more than 1 player
	//check player 1 and player 2 interaction
	if( m_num_players > 1)
	{
		//if player 1 attack box is active, player 2 attack box inactive
		if(player_attack_boxes_ptrs[0]->active && !player_attack_boxes_ptrs[1]->active)
		{
			//check collision between player 1 attack box and player 2
		}
		//if player 2 attack box is active, player 1 attack box inactive
		else if(player_attack_boxes_ptrs[1]->active && !player_attack_boxes_ptrs[0]->active)
		{
			//check collision between player 2 attack box and player 1
		}
	}
	
	//if there are more than 2 players
	//check player 1 and player 3, player 2 and player 3 interactions
	if( m_num_players > 2)
	{
		//if player 1 attack box is active, player 3 attack box inactive
		if(player_attack_boxes_ptrs[0]->active && !player_attack_boxes_ptrs[2]->active)
		{
			//check collision between player 1 attack box and player 3
		}
		//if player 3 attack box is active, player 1 attack box inactive
		else if(player_attack_boxes_ptrs[2]->active && !player_attack_boxes_ptrs[0]->active)
		{
			//check collision between player 3 attack box and player 1
		}
		
		//if player 2 attack box is active, player 3 attack box inactive
		if(player_attack_boxes_ptrs[1]->active && !player_attack_boxes_ptrs[2]->active)
		{
			//check collision between player 2 attack box and player 3
		}
		//if player 3 attack box is active, player 2 attack box inactive
		else if(player_attack_boxes_ptrs[2]->active && !player_attack_boxes_ptrs[1]->active)
		{
			//check collision between player 3 attack box and player 2
			
		}
		
	}
	
	//if there are more than 3 players
	//check player 1 and player 4, player 2 and player 4, player 3 and player 4 interactions
	if( m_num_players > 3)
	{
		//if player 1 attack box is active, player 4 attack box inactive
		if(player_attack_boxes_ptrs[0]->active && !player_attack_boxes_ptrs[3]->active)
		{
			//check collision between player 1 attack box and player 4
		}
		//if player 4 attack box is active, player 1 attack box inactive
		else if(player_attack_boxes_ptrs[3]->active && !player_attack_boxes_ptrs[0]->active)
		{
			//check collision between player 3 attack box and player 1
		}
		
		//if player 2 attack box is active, player 4 attack box inactive
		if(player_attack_boxes_ptrs[1]->active && !player_attack_boxes_ptrs[3]->active)
		{
			//check collision between player 2 attack box and player 4
		}
		//if player 4 attack box is active, player 2 attack box inactive
		else if(player_attack_boxes_ptrs[3]->active && !player_attack_boxes_ptrs[1]->active)
		{
			//check collision between player 4 attack box and player 2
			
		}
		
		//if player 3 attack box is active, player 4 attack box inactive
		if(player_attack_boxes_ptrs[2]->active && !player_attack_boxes_ptrs[3]->active)
		{
			//check collision between player 3 attack box and player 4
		}
		//if player 4 attack box is active, player 3 attack box inactive
		else if(player_attack_boxes_ptrs[3]->active && !player_attack_boxes_ptrs[2]->active)
		{
			//check collision between player 4 attack box and player 3
			
		}
		
	}
}
