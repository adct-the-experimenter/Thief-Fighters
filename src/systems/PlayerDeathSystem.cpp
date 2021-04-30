#include "PlayerDeathSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

#include <cmath>

extern Coordinator gCoordinator;

#include "misc/level_maps.h"


void PlayerDeathSystem::Init()
{
        
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
			render_comp.render = false;
		}
	}
		
	
}

