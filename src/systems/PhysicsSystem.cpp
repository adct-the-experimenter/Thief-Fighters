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

void PushBack(float& posX, float& posY, float& velX, float& velY, float& dt)
{
    float pushBackDist = 2.0f;
    
    float newPosX, newPosY;
    //if going right, push left
    if(velX > 0){newPosX = posX - pushBackDist - (velX * dt);}
    //else if going left, push right
    else if(velX < 0){newPosX = posX + pushBackDist - (velX * dt);}
    else if(velX == 0){newPosX = posX + pushBackDist;}
    
	posX = newPosX;
    
    //if going down, push up
    if(velY > 0){newPosY = posY - pushBackDist - (velY * dt);}
    //else if going up, push down
    else if(velY < 0){newPosY = posY + pushBackDist - (velY * dt);}
    else if(velY == 0){newPosY = posY + pushBackDist; }

    
    posY = newPosY;
}

bool CollisionWithTileDetected(std::uint32_t& tile_x, std::uint32_t& tile_y, std::uint32_t tile_width,
						   float& obj_x, float& obj_y, std::uint32_t obj_width, std::uint32_t& obj_height)
{
	//assuming object has width and height of 30 and it is centered
	
	float objLeftX = obj_x;
	float objRightX = obj_x + obj_width;
	float objTopY = obj_y;
	float objBottomY = obj_y + obj_height;
	
	std::uint32_t tileLeftX = tile_x;
	std::uint32_t tileRightX = tile_x + tile_width;
	std::uint32_t tileTopY = tile_y;
	std::uint32_t tileBottomY = tile_y + tile_width;
	
	//for collision to be true, all conditions must be true. AABB square collsion detection, all
	//The left edge x-position of [A] must be less than the right edge x-position of [B].
    //The right edge x-position of [A] must be greater than the left edge x-position of [B].
    //The top edge y-position of [A] must be less than the bottom edge y-position of [B].
    //The bottom edge y-position of [A] must be greater than the top edge y-position of [B].
    
    if(objBottomY <= tileTopY)
	{
		return false;
	}
	
	if(objTopY >= tileBottomY)
	{
		return false;
	}
    
    if(objRightX <= tileLeftX)
	{
		return false;
	}
	
	if(objLeftX >= tileRightX)
	{
		return false;
	}
	
	return true;
}

bool CollisionWithTileDetected_Alt(std::uint32_t& tile_x, std::uint32_t& tile_y, std::uint32_t tile_width,
						   std::uint32_t& obj_x, std::uint32_t& obj_y, std::uint32_t obj_width, std::uint32_t& obj_height)
{
	//assuming object has width and height of 30 and it is centered
	
	float objLeftX = obj_x;
	float objRightX = obj_x + obj_width;
	float objTopY = obj_y;
	float objBottomY = obj_y + obj_height;
	
	std::uint32_t tileLeftX = tile_x;
	std::uint32_t tileRightX = tile_x + tile_width;
	std::uint32_t tileTopY = tile_y;
	std::uint32_t tileBottomY = tile_y + tile_width;
	
	//for collision to be true, all conditions must be true. AABB square collsion detection, all
	//The left edge x-position of [A] must be less than the right edge x-position of [B].
    //The right edge x-position of [A] must be greater than the left edge x-position of [B].
    //The top edge y-position of [A] must be less than the bottom edge y-position of [B].
    //The bottom edge y-position of [A] must be greater than the top edge y-position of [B].
    
    if(objBottomY <= tileTopY)
	{
		return false;
	}
	
	if(objTopY >= tileBottomY)
	{
		return false;
	}
    
    if(objRightX <= tileLeftX)
	{
		return false;
	}
	
	if(objLeftX >= tileRightX)
	{
		return false;
	}
	
	return true;
}

void PhysicsSystem::Update(float& dt)
{
	
	for (auto const& entity : mEntities)
	{
		auto& rigidBody = gCoordinator.GetComponent<RigidBody2D>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& physics_type_comp = gCoordinator.GetComponent<PhysicsTypeComponent>(entity);
		//auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
		
		// Forces
		auto const& gravity = gCoordinator.GetComponent<Gravity2D>(entity);
		
		switch(physics_type_comp.phy_type)
		{
			case PhysicsType::FIGHTING_GAME:
			{
				//have y axis movement move up or down in circle
				//have x axis movement move left or right
				
				//move transform component by velocity of rigid body multiplied by time
				transform.position.x += rigidBody.velocity.x * dt ;
				transform.position.y += rigidBody.velocity.y * dt;
				
				break;
			}
			default:{break;}
		}
		
	}
	
}


