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
	float width_factor = 1;
	
	for (auto const& entity : mEntities)
	{
			
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& anim_comp = gCoordinator.GetComponent<Animation>(entity);
		auto& render_comp = gCoordinator.GetComponent<RenderModelComponent>(entity);
		
		//update animation
		
		//increment frame count
		m_time_counter += dt;
		
		if(m_time_counter >= 0.25f)
		{
			anim_comp.frame_count++;
			m_time_counter = 0;
		}
		
		switch( anim_comp.anim_actor_type)
		{
			case AnimatedActorType::PLAYER:
			{

				float xdiff = anim_comp.last_position.x - transform.position.x;
				bool south = false; bool north = false;
				bool east = false; bool west = false;
				bool no_move = false;
				
				if(xdiff > 0){west = true;}
				else if(xdiff < 0){east = true;}
				
				float ydiff = anim_comp.last_position.y - transform.position.y;
				if(ydiff > 0){north = true;}
				else if(ydiff < 0){south = true;}
				
				if(xdiff == 0 && ydiff == 0){no_move = true;}
				
				anim_comp.last_position.y = transform.position.y;
				anim_comp.last_position.x = transform.position.x;
				
				if(west)
				{
					anim_comp.horiz_frame_offset = 3;
				}
				else if(east)
				{
					anim_comp.horiz_frame_offset = 0;
				}
				
				//horizontal frame offset refers to horizontal distance between a frame and 
				//the first frame in sprite sheet.
				if(south && !west && !east)
				{
					anim_comp.horiz_frame_offset = 7;
					anim_comp.frame_count = 0;
				}
				else if(north && !west && !east)
				{
					anim_comp.horiz_frame_offset = 6;
					anim_comp.frame_count = 0;
				}
				
				if(anim_comp.attackMode != -1)
				{
					if((anim_comp.attackMode == 1 || anim_comp.attackMode == 2) && anim_comp.frame_count == 0)
					{
						width_factor = 2;
					}
					else
					{
						width_factor = 1;
					}
					anim_comp.vert_frame_offset = anim_comp.attackMode + 1;
					
					if(west)
					{
						anim_comp.horiz_frame_offset = 2;
					}
					else if(east)
					{
						anim_comp.horiz_frame_offset = 0;
					}
					
					if(south && !west && !east)
					{
						anim_comp.horiz_frame_offset = 7;
						anim_comp.frame_count = 0;
						anim_comp.vert_frame_offset = 0;
					}
					else if(north && !west && !east)
					{
						anim_comp.horiz_frame_offset = 6;
						anim_comp.frame_count = 0;
						anim_comp.vert_frame_offset = 0;
					}
					
					//limited to only 2 frame of animation
					if(anim_comp.frame_count == 2)
					{
						//reset attack mode
						anim_comp.attackMode = -1;
						anim_comp.vert_frame_offset = 0;
						width_factor = 1;
					}
				}
				else
				{
					width_factor = 1;
					anim_comp.vert_frame_offset = 0;
					
					//limited to only 3 frames of animation
					if(anim_comp.frame_count == 3 || no_move )
					{
						anim_comp.frame_count = 0;
					}
				}
				
				
				//set render frame based on animation info
			
				//change x position of frame selector
				render_comp.frame_rect.x = (anim_comp.frame_count + anim_comp.horiz_frame_offset)*anim_comp.frame_width;
				render_comp.frame_rect.y = anim_comp.vert_frame_offset*anim_comp.frame_height;
				render_comp.frame_rect.width = width_factor * 30;
				break;
			}
			
			
		}
				
	}
}


