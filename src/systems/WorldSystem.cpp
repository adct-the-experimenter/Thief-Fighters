#include "WorldSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>
#include <cmath>

extern Coordinator gCoordinator;

void WorldSystem::Init()
{
	
}



void WorldSystem::logic(float& dt)
{
	
	auto& player = gCoordinator.GetComponent<Player>(entity)
	auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
	auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
	
	//for all players
		
		//get world that player is in
		//get player location in world
		
		
		
		
		//if player collides with transportation
			//handle transportation of players
			
			//prevent non-camera lead player from inhabiting world alone
			
			//if world is not already in active use, load the tiles for it
			
			//change world that player is in
			//change player location in world
			
			//set which camera player is in
	
			
}

void WorldSystem::render()
{
	//for all players
		
		//get world that player is in
		//get player location in world
		
		//render tiles to camera based on player location
		
		//only render tiles near player
	
}
