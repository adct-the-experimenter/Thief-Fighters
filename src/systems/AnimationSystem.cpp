#include "AnimationSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

#include "misc/media.h"

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
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
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

				Rectangle* frame_rect_ptr = nullptr;
				
				bool south = false; bool north = false;
				bool east = false; bool west = false;
				bool no_move = false;
				
				if(rigidBody.velocity.x > 0){east = true;}
				else if(rigidBody.velocity.x < 0){west = true;}
				
				if(rigidBody.velocity.y > 0){south = true;}
				else if(rigidBody.velocity.y < 0){north = true;}
				
				if(!south && !east && !north && !west){no_move = true;}
				
				
				//if attacking
				if(anim_comp.attackMode != -1)
				{
					//limited to only 2 frame of animation
					if(anim_comp.frame_count == 2)
					{
						anim_comp.frame_count = 0;
					}
					
					if(east)
					{
						anim_comp.face_dir = FaceDirection::EAST;
						//face right attack
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].right_attack_mode_frames[anim_comp.attackMode].frames[anim_comp.frame_count];
					}
					else if(west)
					{
						anim_comp.face_dir = FaceDirection::WEST;
						//face left attack
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].left_attack_mode_frames[anim_comp.attackMode].frames[anim_comp.frame_count];
					}
					
					
					if(south && !west && !east)
					{
						anim_comp.face_dir = FaceDirection::SOUTH;
						
						//face down
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].down_still_frame.frame;
						anim_comp.frame_count = 0;
					}
					else if(north && !west && !east)
					{
						anim_comp.face_dir = FaceDirection::NORTH;
						
						//face up
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].up_still_frame.frame;
						anim_comp.frame_count = 0;
					}
					
					if(no_move)
					{
						if(anim_comp.face_dir == FaceDirection::EAST)
						{
							frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].right_attack_mode_frames[anim_comp.attackMode].frames[anim_comp.frame_count];
						}
						else if(anim_comp.face_dir == FaceDirection::WEST)
						{
							frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].left_attack_mode_frames[anim_comp.attackMode].frames[anim_comp.frame_count];
						}
					}
					
					
				}
				//if not attacking
				else
				{
					
					if(west)
					{
						anim_comp.face_dir = FaceDirection::WEST;
						//face left
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].left_walk_frames.frames[anim_comp.frame_count];
					}
					else if(east)
					{
						anim_comp.face_dir = FaceDirection::EAST;
						//face right
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].right_walk_frames.frames[anim_comp.frame_count];
					}
					
					//horizontal frame offset refers to horizontal distance between a frame and 
					//the first frame in sprite sheet.
					if(south && !west && !east)
					{
						anim_comp.face_dir = FaceDirection::SOUTH;
						//face down
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].down_still_frame.frame;
						anim_comp.frame_count = 0;
					}
					else if(north && !west && !east)
					{
						anim_comp.face_dir = FaceDirection::NORTH;
						//face up
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].up_still_frame.frame;
						anim_comp.frame_count = 0;
					}
					
					
					//limited to only 2 frames of animation
					if(anim_comp.frame_count == 2 || no_move )
					{
						anim_comp.frame_count = 0;
						
						if(no_move)
						{
							if(anim_comp.face_dir == FaceDirection::EAST)
							{
								frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].right_still_frame.frame;
							}
							else if(anim_comp.face_dir == FaceDirection::WEST)
							{
								frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].left_still_frame.frame;
							}
						}
					}
					
				}
				
				//if there is a frame to change to
				if(frame_rect_ptr)
				{
					render_comp.frame_rect = *frame_rect_ptr;
				}
								
				break;
			}
			
			
		}
				
	}
}


