#include "AnimationSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void AnimationSystem::Init()
{
	m_time_counter = 0;
}

//face direction order: SOUTH=0,SOUTHWEST,WEST,NORTHWEST,NORTH,NORTHEAST,EAST,SOUTHEAST

void AnimationSystem::Update(float& dt)
{
	
	for (auto const& entity : mEntities)
	{
			
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& anim_comp = gCoordinator.GetComponent<Animation>(entity);
		auto& render_comp = gCoordinator.GetComponent<RenderModelComponent>(entity);
		
		//update animation
		//anim_comp.info.animFrameCounter++;
		
		
		if (anim_comp.info.animFrameCounter >= anim_comp.info.anims[0].frameCount) 
		{
			anim_comp.info.animFrameCounter = 0;
		}
		
	}
}


