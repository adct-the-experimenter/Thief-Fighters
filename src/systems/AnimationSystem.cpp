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

static int framesCounter = 0;
static int framesSpeed = 1; 

void AnimationSystem::Update(float& dt)
{
	//increment frame counter
	framesCounter++;
       
    int iterator = 0;
    
    bool increment_frame_count = false;
    
    //update animation
		
	if (framesCounter >= (60/framesSpeed))
	{
		framesCounter = 0;
		
		increment_frame_count = true;
		
					
	}
    
	for (auto const& entity : mEntities)
	{
			
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& anim_comp = gCoordinator.GetComponent<Animation>(entity);
		auto& render_comp = gCoordinator.GetComponent<RenderModelComponent>(entity);
		
		if(increment_frame_count)
		{
			anim_comp.frame_count++;
		}
		
		if (anim_comp.frame_count > 1){anim_comp.frame_count = 0;}
		
		
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
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].down_attack_mode_frames[anim_comp.attackMode].frames[anim_comp.frame_count];
						anim_comp.frame_count = 0;
					}
					else if(north && !west && !east)
					{
						anim_comp.face_dir = FaceDirection::NORTH;
						
						//face up
						frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].up_attack_mode_frames[anim_comp.attackMode].frames[anim_comp.frame_count];
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
					if(anim_comp.frame_count == 2)
					{
						anim_comp.frame_count = 0;
					}
					
					//if player is hurt
					if(anim_comp.hurt)
					{
						if(west)
						{
							anim_comp.face_dir = FaceDirection::WEST;
							
							//face left
							frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].left_hurt_frames.frames[anim_comp.frame_count];
						}
						else if(east)
						{
							anim_comp.face_dir = FaceDirection::EAST;
							//face right hurt
							frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].right_hurt_frames.frames[anim_comp.frame_count];
						}
						else if(south && !west && !east)
						{
							anim_comp.face_dir = FaceDirection::SOUTH;
							
							//need to add down hurt frames, for now just use right hurt frame
							frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].right_hurt_frames.frames[anim_comp.frame_count];
							anim_comp.frame_count = 0;
						}
						else if(north && !west && !east)
						{
							anim_comp.face_dir = FaceDirection::NORTH;
							
							//need to add up hurt frames, for now just use left hurt frames
							frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].left_hurt_frames.frames[anim_comp.frame_count];
							anim_comp.frame_count = 0;
						}
						if(no_move)
						{
							if(anim_comp.face_dir == FaceDirection::EAST || anim_comp.face_dir == FaceDirection::SOUTH)
							{
								frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].right_hurt_frames.frames[anim_comp.frame_count];
							}
							else if(anim_comp.face_dir == FaceDirection::WEST || anim_comp.face_dir == FaceDirection::NORTH)
							{
								frame_rect_ptr = &character_frame_animations[render_comp.char_texture_index].left_hurt_frames.frames[anim_comp.frame_count];
							}
						}
					}
					//else if player is not hurt
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
						if(no_move )
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
					
					
					
					
				}
				
				//if there is a frame to change to
				if(frame_rect_ptr)
				{
					render_comp.frame_rect = *frame_rect_ptr;
				}
								
				break;
			}
			
			
		}
			
		iterator++;	
	}
	
	//reset increment frame count
	increment_frame_count = false;
}


