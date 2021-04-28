#include "AttackPowerMechanicSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void AttackPowerMechanicSystem::Init()
{
	
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
			player.regularAttackButtonPressed = false;
		}
		
		//change and/or activate current power based on input
		if(player.powerButtonPressed && player.requested_power != -1 &&
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
		for(size_t i = 0; i < 7; i++)
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

