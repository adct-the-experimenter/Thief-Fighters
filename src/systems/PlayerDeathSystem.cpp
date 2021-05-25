#include "PlayerDeathSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

#include <cmath>

extern Coordinator gCoordinator;

#include "misc/level_maps.h"


void PlayerDeathSystem::Init(std::uint8_t num_players)
{
	winning_player = -1;
	
	players_alive.reset();
	for(std::uint8_t i = 0; i < num_players; i++)
	{
		players_alive[i] = 1;
	}
}


void PlayerDeathSystem::Update()
{
			
	//for every entity
	for (auto const& entity : mEntities)
	{
		auto& render_comp = gCoordinator.GetComponent<RenderModelComponent>(entity);
		auto& player = gCoordinator.GetComponent<Player>(entity);
		
		if(!player.alive && render_comp.render)
		{
			players_alive[player.player_num - 1] = 0;
			render_comp.render = false;
		}
	}
	
	//check if only 1 player is alive
	if(players_alive.count() == 1 && winning_player == -1)
	{
		std::cout << "\nOne player won!\n";
		
		one_player_won = true;
		
		//set winning player
		for(std::uint8_t i = 0; i < 8; i++)
		{
			if(players_alive[i] == 1)
			{
				winning_player = i;
			}
		}
	}
	 
	
}

bool PlayerDeathSystem::OnePlayerWon(){return one_player_won;}
	
std::int8_t PlayerDeathSystem::GetPlayerWhoWon(){return winning_player;}
