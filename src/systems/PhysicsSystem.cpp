#include "PhysicsSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include "misc/level_maps.h"
#include <iostream>
#include <cmath> //for rounding
#include <array>

extern Coordinator gCoordinator;


void PhysicsSystem::Init()
{
	
}

static void PushBack(float& posX, float& posY, float& velX, float& velY, float& dt)
{
    float pushBackDist = 2.0f;
    
    float newPosX, newPosY;
    //if going right, push left
    if(velX > 0){newPosX = posX - pushBackDist - (velX * dt);}
    //else if going left, push right
    else if(velX < 0){newPosX = posX + pushBackDist - (velX * dt);}
    else if(velX == 0){newPosX = posX;}
    
	posX = newPosX;
    velX = 0;
    
    //if going down, push up
    if(velY > 0){newPosY = posY - pushBackDist - (velY * dt);}
    //else if going up, push down
    else if(velY < 0){newPosY = posY + pushBackDist - (velY * dt);}
    else if(velY == 0){newPosY = posY; }

    
    posY = newPosY;
    velY = 0;
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

static void CheckCollisionWithPlatforms(float& obj_x, float& obj_y, 
										float& obj_vx, float& obj_vy, 
										float& dt, 
										std::uint32_t& obj_width,
										std::uint32_t& obj_height)
{
	//for each rectangle platform in mains stage
	for(size_t i = 0; i < main_stage.collision_rect_array.size(); i++)
	{
		//if player(obj) collides with a platforms
		if(CollisionWithPlatformDetected(main_stage.collision_rect_array[i].rect,
						   obj_x, obj_y, obj_width, obj_height)
			)
		{
			//if player is on top of object
			if(obj_y + obj_height - 1 <= main_stage.collision_rect_array[i].rect.y)
			{
				obj_vy = 0;
			}
			else
			{
				//push back player 
				PushBack(obj_x, obj_y, 
					obj_vx, obj_vy, 
					dt);
			}
			
		}
	}
}

static void CheckCollisionWithLevelBounds(float& obj_x, float& obj_y, 
										float& obj_vx, float& obj_vy, 
										float& dt, 
										std::uint32_t& obj_width,
										std::uint32_t& obj_height)
{
	//if go to the right of the level bound
	if(obj_x + obj_width >= 720)
	{
		//push back player 
		PushBack(obj_x, obj_y, 
			obj_vx, obj_vy, 
			dt);
			
		obj_x = 720 - obj_width - 1;
	}
	//if go to the left of level bound
	else if(obj_x <= 0)
	{
		//push back player 
		PushBack(obj_x, obj_y, 
			obj_vx, obj_vy, 
			dt);
		
		obj_x = 0 + 1;
	}
	
	//if go above up bound
	if(obj_y + 2*obj_height <= 0)
	{
		//push back player 
		PushBack(obj_x, obj_y, 
			obj_vx, obj_vy, 
			dt);
		obj_y = 0 + 1;
	}
	
	//if go below down bound
	else if(obj_y + obj_height >= 360)
	{
		//push back player 
		PushBack(obj_x, obj_y, 
			obj_vx, obj_vy, 
			dt);
		
		obj_y = 360 - obj_height - 1;
	}
}

void PhysicsSystem::Update(float& dt)
{
	
	for (auto const& entity : mEntities)
	{
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& physics_type_comp = gCoordinator.GetComponent<PhysicsTypeComponent>(entity);
		auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
		
		// Forces
		auto const& gravity = gCoordinator.GetComponent<Gravity2D>(entity);
		
		switch(physics_type_comp.phy_type)
		{
			case PhysicsType::PLATFORMER:
			{
				//account for acceleration due to gravity to rigid body velocity
				//rigidBody.velocity.x += gravity.force.x * dt;
				
				float jumpVel = rigidBody.velocity.y*3;
				
				if(jumpVel < 0)
				{
					//std::cout << "\nplayer is jumping.\n";
					
					rigidBody.velocity.y += 3*gravity.force.y * dt;
				}
				else
				{
					rigidBody.velocity.y += 3*gravity.force.y * dt;
				
					CheckCollisionWithPlatforms(transform.position.x, transform.position.y,
														  rigidBody.velocity.x, rigidBody.velocity.y,
														  dt,
														  collisionBox.width, collisionBox.height);
				}
				
				
				CheckCollisionWithLevelBounds(transform.position.x, transform.position.y,
														  rigidBody.velocity.x, rigidBody.velocity.y,
														  dt,
														  collisionBox.width, collisionBox.height);
				
				
				
				//move transform component by velocity of rigid body multiplied by time
				//std::cout << "In physics system, player rigid body velocity: " << rigidBody.velocity.x << std::endl;
				transform.position.x += 3*rigidBody.velocity.x * dt;
				transform.position.y += (rigidBody.velocity.y + jumpVel) * dt;
				
				break;
			}
			default:{break;}
		}
		
	}
	
}


