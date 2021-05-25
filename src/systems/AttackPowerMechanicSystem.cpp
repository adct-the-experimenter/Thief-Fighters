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
		player_attack_damage_factor_ptrs[i] = nullptr;
		player_hurt_invincible_ptrs[i] = nullptr;
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
		
		player_hurt_invincible_ptrs[player.player_num - 1] = &player.hurt_invincible;
		
		player_attack_damage_factor_ptrs[player.player_num - 1] = &player.damage_factor;
		
		//set collected power for player
		player.collected_powers[player.current_power] = 1;
	}
	
	m_num_players = num_players;
}



static float speed_boost = 20.0f;

void AttackPowerMechanicSystem::HandlePowerActivation(float& dt)
{
	
	for (auto const& entity : mEntities)
	{
		
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& player = gCoordinator.GetComponent<Player>(entity);
		auto& animation = gCoordinator.GetComponent<Animation>(entity);
		auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
		
		//if player pressed attack button and isn't in hurting state and alive.
		if(player.regularAttackButtonPressed && player.alive && !player.taking_damage && player.state != PlayerState::HURTING)
		{
			//if attack box is not active i.e. player is not attacking already
			if(!player.attack_box.active)
			{
				animation.attackMode = 0;
			
				//initialize attack box
				
				player.attack_box.active = true;
				player.state = PlayerState::ATTACKING;
				
			}
			
			player.regularAttackButtonPressed = false;
		}
		
		//change and/or activate current power based on input
		else if(player.powerButtonPressed && player.requested_power < 8 && player.alive && !player.taking_damage && player.state != PlayerState::HURTING)
		{
			
			//std::cout << "Player " << int(player.player_num) << " requested this power: " << int(player.requested_power) << std::endl;
			//check which power player is requesting
			//change to power requested if player has this power
			if( player.requested_power != -1 && player.collected_powers[player.requested_power])
			{
				//activate power, if not active
				if( !player.powers_activated[player.requested_power])
				{
					player.current_power = player.requested_power;
					player.state = PlayerState::ATTACKING;
					
				}
				
				
			}
			else
			{
				//reset back to the power that the player has used previously saved in current power.
				player.requested_power = player.current_power;
			}
			
			//activate power if not activated
			if( !player.powers_activated[player.current_power])
			{
				player.state = PlayerState::ATTACKING;
				animation.attackMode = player.current_power + 1;
				
				//std::cout << "current power of player " << int(player.player_num) << ": " << int(player.current_power) << std::endl;
				player.powers_activated[player.current_power] = 1;
				
				//set attack box or player speed based on power
				switch(player.current_power)
				{
					// sneak
					case 0:
					{
						//do nothing, no attack box.
						break;
					}
					//dash
					case 1:
					{
						//increase horizontal speed
						rigidBody.velocity.x = speed_boost*rigidBody.velocity.x;
						
						//cancel any attack if dashing
						player.attack_box.active = false;
						
						break;
					}
					//shield
					case 2:
					{
						//activate collision box
						player.attack_box.active = true;
						
						player.attack_box.collisionBox.x = -5;
						player.attack_box.collisionBox.y = -5;
						//it won't collide with another attack box
						
						break;
					}
					//chunks
					case 3:
					{
						player.attack_box.active = true;

						//decrease horizontal speed
						rigidBody.velocity.x = 0.5*rigidBody.velocity.x;
						break;
					}
					//big
					case 4:
					{
						player.attack_box.active = true;
						
						//move player up
						transform.position.y -= 60;
						
						//change player collision box to match the large size
						collisionBox.width = 60;
						collisionBox.height = 120;
						
						//set horizontal speed to zero
						rigidBody.velocity.x = 0.0f;
						break;
					}
				}
			}
			
			//reset power button pressed
			player.powerButtonPressed = false;
			
		}
		
		//cool down timers
		
		//regular attack cooldown
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
				//reset animation
				animation.attackMode = -1;
				
				player.state = PlayerState::IDLE;
			}
		}
		
		//cooldown for special power if activated
		for(size_t i = 0; i < 8; i++)
		{
			
			//if special power is active
			if(player.powers_activated.test(i))
			{
				
				player.sp_attack_cooldown_timer_val_array[i] += dt;
								
				//reset attack box or player speed based on power
				switch(i)
				{
					// sneak
					case 0:
					{
						
						//if more than 2 seconds have passed
						if(player.sp_attack_cooldown_timer_val_array[i] >= 3)
						{
							//reset bitset for power activated if cooldown time has finished
							player.powers_activated[i] = 0;
							//reset cooldown timer value
							player.sp_attack_cooldown_timer_val_array[i] = 0;
							//reset animation for attack mode
							animation.attackMode = -1;
							
							player.state = PlayerState::IDLE;
						}
						break;
					}
					//dash
					case 1:
					{
						//return to original horizontal speed after 1 second
						if(player.sp_attack_cooldown_timer_val_array[i] >= 1)
						{
							rigidBody.velocity.x = (1/speed_boost)*rigidBody.velocity.x;
							
							//reset bitset for power activated if cooldown time has finished
							player.powers_activated[i] = 0;
							//reset cooldown timer value
							player.sp_attack_cooldown_timer_val_array[i] = 0;
							//reset animation for attack mode
							animation.attackMode = -1;
							
							player.state = PlayerState::IDLE;
						}
						
						
						break;
					}
					//shield
					case 2:
					{
						//if more than 4 seconds have passed
						if(player.sp_attack_cooldown_timer_val_array[i] >= 4)
						{
							player.attack_box.active = false;
							
							//reset bitset for power activated if cooldown time has finished
							player.powers_activated[i] = 0;
							//reset cooldown timer value
							player.sp_attack_cooldown_timer_val_array[i] = 0;
							
							//reset animation for attack mode
							animation.attackMode = -1;
							
							player.state = PlayerState::IDLE;
							
						}
						
						break;
					}
					//chunks
					case 3:
					{
						//if more than 4 seconds have passed
						if(player.sp_attack_cooldown_timer_val_array[i] >= 4)
						{
							//decrease horizontal speed
							rigidBody.velocity.x = 2*rigidBody.velocity.x;
							
							player.attack_box.active = false;
							
							//reset bitset for power activated if cooldown time has finished
							player.powers_activated[i] = 0;
							//reset cooldown timer value
							player.sp_attack_cooldown_timer_val_array[i] = 0;
							
							//reset animation for attack mode
							animation.attackMode = -1;
							
							player.state = PlayerState::IDLE;
							
						}
						
						break;
					}
					//big
					case 4:
					{
						//if more than 5 seconds have passed
						if(player.sp_attack_cooldown_timer_val_array[i] >= 1)
						{
							
							if(player.sp_attack_cooldown_timer_val_array[i] >= 4)
							{
								//reset bitset for power activated if cooldown time has finished
								player.powers_activated[i] = 0;
								//reset cooldown timer value
								player.sp_attack_cooldown_timer_val_array[i] = 0;
							}
							else
							{
								//reset speed
								rigidBody.velocity.x = rigidBody.velocity.x;
								
								
								//change player collision box to match the normal size
								collisionBox.width = 30;
								collisionBox.height = 60;
								
								player.attack_box.active = false;
								
								
								//reset animation for attack mode
								animation.attackMode = -1;
								
								player.state = PlayerState::IDLE;
							}
							
							
							
							
							
						}
						
						break;
					}
				}
				
			}
		}
		
		
		
		
		
	}
	
}

void AttackPowerMechanicSystem::PerformNeededPowerTransactions()
{
	
	//power steal mechanic here
	
	for (auto const& entity : mEntities)
	{
		auto& player = gCoordinator.GetComponent<Player>(entity);
		
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


void AttackPowerMechanicSystem::MoveAttackBoxesWithPlayer(float& dt)
{
	for (auto const& entity : mEntities)
	{
		
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& player = gCoordinator.GetComponent<Player>(entity);
		auto& animation = gCoordinator.GetComponent<Animation>(entity);
		auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
		
		if(player.attack_box.active)
		{
			switch(animation.attackMode)
			{
				//regular attack
				case 0:
				{
					float offset_x = 0;
			
					if(animation.face_dir == FaceDirection::EAST)
					{
						offset_x = player.attack_box_offset;
					}
					else if(animation.face_dir == FaceDirection::WEST)
					{
						offset_x = -player.attack_box_offset;
					}
					
					//activate collision box
					player.attack_box.collisionBox.x = transform.position.x + offset_x; 
					player.attack_box.collisionBox.y = transform.position.y + 20;
					player.attack_box.collisionBox.width = 30;
					player.attack_box.collisionBox.height = 30;
					
					player.attack_box.player_num = player.player_num;
					
					break;
				}
				//chunks
				case 4:
				{
					int offset_x = 0;
			
					if(animation.face_dir == FaceDirection::EAST)
					{
						offset_x = 10;
					}
					else if(animation.face_dir == FaceDirection::WEST)
					{
						offset_x = -10;
					}
					
					//activate collision box
					player.attack_box.collisionBox.x = transform.position.x + offset_x; 
					player.attack_box.collisionBox.y = transform.position.y ;
					player.attack_box.collisionBox.width = 50;
					player.attack_box.collisionBox.height = 50;
					
					player.attack_box.player_num = player.player_num;
					break;
				}
				//big
				case 5:
				{
					int offset_x = 0;
			
					if(animation.face_dir == FaceDirection::EAST)
					{
						offset_x = 10;
					}
					else if(animation.face_dir == FaceDirection::WEST)
					{
						offset_x = -10;
					}
					
					//activate attack collision box
					player.attack_box.collisionBox.x = transform.position.x + offset_x; 
					player.attack_box.collisionBox.y = transform.position.y ;
					player.attack_box.collisionBox.width = 60;
					player.attack_box.collisionBox.height = 120;
					
					player.attack_box.player_num = player.player_num;
				}
				default:{break;}
			}
			
			
		}
		
		
		
	}
	
}

void AttackPowerMechanicSystem::CollisionDetectionBetweenPlayers()
{
	int player_a_num = 0;
	int player_b_num = 0;
	
	//if there is more than 1 player
	//check player 1 and player 2 interaction
	if( m_num_players > 1)
	{
		player_a_num = 1;
		player_b_num = 2;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
	}
	
	//if there are more than 2 players
	//check player 1 and player 3, player 2 and player 3 interactions
	if( m_num_players > 2 )
	{
		AttackEvent attack_event;
		
		//check player 1 and 3 interaction
		
		player_a_num = 1;
		player_b_num = 3;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 2 and player 3 interaction
		
		player_a_num = 2;
		player_b_num = 3;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
	}
	
	//if there are more than 3 players
	//check player 1 and player 4, player 2 and player 4, player 3 and player 4 interactions
	if( m_num_players > 3)
	{
		
		//check player 1 and player 4 interaction
		player_a_num = 1;
		player_b_num = 4;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 2 and player 4 interaction
		player_a_num = 2;
		player_b_num = 4;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 3 and player 4 interaction
		player_a_num = 3;
		player_b_num = 4;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
	}
	
	//if there are 5 players
	if(m_num_players > 4)
	{
		
		//check player 1 and player 5 interaction
		player_a_num = 1;
		player_b_num = 5;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 2 and player 5 interaction
		player_a_num = 2;
		player_b_num = 5;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 3 and player 5 interaction
		player_a_num = 3;
		player_b_num = 5;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 4 and player 5 interaction
		player_a_num = 4;
		player_b_num = 5;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
	}
	
	//if there are 6 players
	if(m_num_players > 5)
	{
		
		//check player 1 and player 6 interaction
		player_a_num = 1;
		player_b_num = 6;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 2 and player 6 interaction
		player_a_num = 2;
		player_b_num = 6;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 3 and player 6 interaction
		player_a_num = 3;
		player_b_num = 6;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 4 and player 6 interaction
		player_a_num = 4;
		player_b_num = 6;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 5 and player 6 interaction
		player_a_num = 5;
		player_b_num = 6;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
	}
	
	//if there are 7 players
	if(m_num_players > 6)
	{		
		//check player 1 and player 7 interaction
		player_a_num = 1;
		player_b_num = 7;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 2 and player 7 interaction
		player_a_num = 2;
		player_b_num = 7;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 3 and player 7 interaction
		player_a_num = 3;
		player_b_num = 7;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 4 and player 7 interaction
		player_a_num = 4;
		player_b_num = 7;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 5 and player 7 interaction
		player_a_num = 5;
		player_b_num = 7;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 6 and player 7 interaction
		player_a_num = 6;
		player_b_num = 7;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
	}
	//if there are 8 players
	if(m_num_players > 7)
	{
		AttackEvent attack_event;
		
		//check player 1 and player 8 interaction
		player_a_num = 1;
		player_b_num = 8;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 2 and player 8 interaction
		player_a_num = 2;
		player_b_num = 8;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 3 and player 8 interaction
		player_a_num = 3;
		player_b_num = 8;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 4 and player 8 interaction
		player_a_num = 4;
		player_b_num = 8;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 5 and player 8 interaction
		player_a_num = 5;
		player_b_num = 8;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 6 and player 8 interaction
		player_a_num = 6;
		player_b_num = 8;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
		
		//check player 7 and player 8 interaction
		player_a_num = 7;
		player_b_num = 8;
		AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(player_a_num,player_b_num);
	}
	
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

void AttackPowerMechanicSystem::HandlePossibleCollisionBetweenPlayers(int& player_a_num, int& player_b_num)
{
	AttackEvent attack_event;
	attack_event = AttackPowerMechanicSystem::CheckCollisionBetween2Players(player_a_num, player_b_num);
	
	//if attack happened, and victim is not already in state of taking damage or in hurt invicible state
	if(attack_event.attack && !*player_taking_damage_state_ptrs[attack_event.player_num_victim - 1] && !*player_hurt_invincible_ptrs[attack_event.player_num_victim - 1] )
	{
		//std::cout << "Player " << attack_event.player_num_attacker << "took away 10 HP from player " << attack_event.player_num_victim << std::endl;
		
		//decrease health of victim player
		*player_health_ptrs[attack_event.player_num_victim - 1] -= 10*(*player_attack_damage_factor_ptrs[attack_event.player_num_attacker - 1]);
		//set last hit by player variable for victim player
		*player_last_hit_by_ptrs[attack_event.player_num_victim - 1] = attack_event.player_num_attacker;
		
		*player_taking_damage_state_ptrs[attack_event.player_num_victim - 1] = true;
		
		//knock back
		float knockback = 3*(*player_attack_damage_factor_ptrs[attack_event.player_num_attacker - 1]);
		float sign = 1;
		
		if(player_position_ptrs[attack_event.player_num_victim - 1]->x < 
			player_position_ptrs[attack_event.player_num_attacker - 1]->x)
		{
			sign = -1;
		}
		
		player_position_ptrs[attack_event.player_num_victim - 1]->x += sign*knockback;
		
	}
	else
	{
		*player_taking_damage_state_ptrs[player_a_num - 1] = false;
		*player_taking_damage_state_ptrs[player_b_num - 1] = false;
	}
}

void AttackPowerMechanicSystem::ReactToCollisions(float& dt)
{
	for(auto const& entity: mEntities)
	{
		
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& player = gCoordinator.GetComponent<Player>(entity);
		auto& animation = gCoordinator.GetComponent<Animation>(entity);
		//auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
		
		//if player is in state of taking damage 
		//activate hurt animation and keep them from moving
		if(player.taking_damage)
		{
			player.taking_damage = false;
			player.hurt_invincible = true;
			
			//reset animation
			animation.attackMode = -1;
			animation.frame_count = 0;
			animation.hurt = true;
			
			//stop player from moving
			rigidBody.velocity.x = 0.0f;
			rigidBody.velocity.y = 0.0f;
			
			player.hurt_anim_time_count = 0;
			player.hurt_anim_time_count += dt;
			
			player.state = PlayerState::HURTING;
		
		}
		else
		{
			player.hurt_anim_time_count += dt;
			
			//if 2 seconds have passed, stop hurt animation
			if(player.hurt_anim_time_count >= 2)
			{
				animation.hurt = false;
				player.hurt_anim_time_count = 0;
				player.state = PlayerState::IDLE;
				player.hurt_invincible = false;
			}
			
		}
		
	}
}

static bool debugRenderAttackBox = false;
static bool debugDrawPowerRequest = false;

void AttackPowerMechanicSystem::DebugRender()
{
	for (auto const& entity : mEntities)
	{
		
		auto& player = gCoordinator.GetComponent<Player>(entity);
		
		if(player.attack_box.active && debugRenderAttackBox)
		{
			DrawRectangleRec(player.attack_box.collisionBox, RED);
		}
		
		
		
		if(debugDrawPowerRequest)
		{
			std::string req_num_str = std::to_string(player.requested_power);
			DrawText(req_num_str.c_str(), 100 + 20*player.player_num, 20, 20, RED); 
		}
		
	}
}
