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

static void PushUp(float& posY, float& velY, float& dt)
{
    float pushBackDist = 2.0f;
    
    float newPosY = 0;
    newPosY = posY - abs(velY * dt) - pushBackDist;

    
    posY = newPosY;
    //velY = 0;
}

static void PushDown(float& posY, float& velY, float& dt)
{
    float pushBackDist = 1.0f;
    
    float newPosY = 0;
    newPosY = posY + abs(velY * dt) + pushBackDist;

    
    posY = newPosY;
    velY = 0;
}

static void PushLeft(float& posX, float& velX, float& dt)
{
    float pushBackDist = 1.0f;
    
    float newPosX;
    newPosX = posX - abs(velX * dt) - pushBackDist;
    
	posX = newPosX;
    velX = 0;
    
}

static void PushRight(float& posX, float& velX, float& dt)
{
    float pushBackDist = 1.0f;
    
    float newPosX;
    newPosX = posX + abs(velX * dt) + pushBackDist;
    
	posX = newPosX;
    velX = 0;
    
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
										std::uint32_t& obj_height,
										bool& grounded)
{
	//for each rectangle platform in mains stage
	for(size_t i = 0; i < main_stage.collision_rect_array.size(); i++)
	{
		if(!main_stage.collision_rect_array[i].initialized)
		{
			//break out of loop
			break;
		}
		
		//if player(obj) collides with a platforms
		if(CollisionWithPlatformDetected(main_stage.collision_rect_array[i].rect,
						   obj_x, obj_y, obj_width, obj_height)
			)
		{
			//if player is not grounded and on top of a platform
			if(!grounded && (obj_y + obj_height - 1 <= main_stage.collision_rect_array[i].rect.y))
			{
				obj_vy = 0;
				obj_y = main_stage.collision_rect_array[i].rect.y - obj_height;
				grounded = true;
			}
				
			//if on the ground
			if(grounded)
			{
				//if player is on top of platform
				if(obj_y + obj_height - 1 <= main_stage.collision_rect_array[i].rect.y)
				{
					obj_vy = 0;
					obj_y = main_stage.collision_rect_array[i].rect.y - obj_height;
					
				}
				else
				{
					
					//if player is to the left of platform 
					if(obj_x + obj_width - 1 <= main_stage.collision_rect_array[i].rect.x + 20)
					{
						//PushLeft(obj_x,obj_vx,dt);
						obj_vx = 0;
						obj_x = main_stage.collision_rect_array[i].rect.x - obj_width;
					}
					//else if player is to the right of platform
					else if(obj_x + 1 >= main_stage.collision_rect_array[i].rect.x + main_stage.collision_rect_array[i].rect.width - 20)
					{
						//PushRight(obj_x,obj_vx,dt);
						obj_vx = 0;
						obj_x = main_stage.collision_rect_array[i].rect.x + main_stage.collision_rect_array[i].rect.width;
					}
					else
					{
						//push back player 
						//PushBack(obj_x, obj_y, 
						//obj_vx, obj_vy, 
						//dt);
						
						PushUp(obj_y,obj_vy,dt);
						
					}
											
				}
			}
			//else if not grounded
			else
			{
				
				//std::cout << "\nin air.\n";
				
				/*
				std::cout << "player right: " << obj_x + obj_width - 1 << std::endl;
				std::cout << "player left: " << obj_x + 1 << std::endl;
				
				std::cout << "platform left:" << main_stage.collision_rect_array[i].rect.x << std::endl;
				std::cout << "platform right:" << main_stage.collision_rect_array[i].rect.x + main_stage.collision_rect_array[i].rect.width << std::endl;
				std::cout << "platform up:" << main_stage.collision_rect_array[i].rect.y  << std::endl;
				std::cout << "platform down:" << main_stage.collision_rect_array[i].rect.y + main_stage.collision_rect_array[i].rect.height << std::endl;
				*/
				
				//if player is to the left of platform 
				if(obj_x + obj_width - 1 <= main_stage.collision_rect_array[i].rect.x + 20)
				{
					//std::cout << "\nplayer below platform to the left. Push left.\n";
					//PushLeft(obj_x,obj_vx,dt);
					obj_vx = 0;
					obj_x = main_stage.collision_rect_array[i].rect.x - obj_width;
				}
				//else if player is to the right of platform
				else if(obj_x + 1 >= main_stage.collision_rect_array[i].rect.x + main_stage.collision_rect_array[i].rect.width - 20)
				{
					//std::cout << "\nplayer below platform to the right. Push right.\n";
					//PushRight(obj_x,obj_vx,dt);
					obj_vx = 0;
					obj_x = main_stage.collision_rect_array[i].rect.x + main_stage.collision_rect_array[i].rect.width;
				}
				
				//if player is on top of platform
				if(obj_y + obj_height - 2 <= main_stage.collision_rect_array[i].rect.y + 10)
				{
					//std::cout << "\nplayer above platform. Push up.\n";
					//PushUp(obj_y,obj_vy,dt);
					obj_vy = 0;
					obj_y = main_stage.collision_rect_array[i].rect.y - obj_height;
					grounded = true;
				}
				else
				{
					//std::cout << "\nplayer below platform. Push down.\n";
					PushDown(obj_y,obj_vy,dt);
				}
					
				
			}
			

			
		}
	}
}

static bool CollisionWithTileDetected(float tile_x,float tile_y,
						   float& obj_x, float& obj_y, std::uint32_t obj_width, std::uint32_t& obj_height)
{
	//assuming object has width and height of 30 and it is centered
	float tile_width = 30;
	float tile_height = 30;
	
	float objLeftX = obj_x;
	float objRightX = obj_x + obj_width;
	float objTopY = obj_y;
	float objBottomY = obj_y + obj_height;
	
	std::uint32_t rectLeftX = tile_x;
	std::uint32_t rectRightX = tile_x + tile_width;
	std::uint32_t rectTopY = tile_y;
	std::uint32_t rectBottomY = tile_y + tile_height;
	
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

static void CheckCollisionWithTiles(float& obj_x, float& obj_y, 
										float& obj_vx, float& obj_vy, 
										float& dt, 
										std::uint32_t& obj_width,
										std::uint32_t& obj_height,
										bool& grounded,
										World* world_ptr)
{
	
	//calculate tile that object is on
	size_t num_tile_horizontal = 220;
	
	
	size_t horiz_index = trunc(obj_x / 30 );
	size_t vert_index = trunc((obj_y + 30) / 30 ) * num_tile_horizontal;
	
	
	size_t object_tile_index = horiz_index + vert_index; 
	
	std::array <size_t,9> tiles_around_object;
	
	if(object_tile_index > num_tile_horizontal)
	{
		tiles_around_object[0] = object_tile_index - num_tile_horizontal - 1;
		tiles_around_object[1] = object_tile_index - num_tile_horizontal;
		tiles_around_object[2] = object_tile_index - num_tile_horizontal + 1;
	}
	else
	{
		tiles_around_object[0] = 0;
		tiles_around_object[1] = 0;
		tiles_around_object[2] = 0;
	}
	
	if(object_tile_index > 0)
	{
		tiles_around_object[3] = object_tile_index - 1;
	}
	else
	{
		tiles_around_object[3] = 0;
	}
	
	tiles_around_object[4] = object_tile_index;
	
	tiles_around_object[5] = object_tile_index + 1;
	
	if(object_tile_index < num_tile_horizontal*219)
	{
		tiles_around_object[6] = object_tile_index + num_tile_horizontal - 1;
		tiles_around_object[7] = object_tile_index + num_tile_horizontal;
		tiles_around_object[8] = object_tile_index + num_tile_horizontal + 1;
	}
	else
	{
		tiles_around_object[6] = num_tile_horizontal*220 - 3;
		tiles_around_object[7] = num_tile_horizontal*220 - 2;
		tiles_around_object[8] = num_tile_horizontal*220 - 1;
	}
	
	
	
	//for tiles
	for(size_t i = 0; i < tiles_around_object.size(); i++)
	{
		size_t& tile_index = tiles_around_object[i];
		
		
		if(world_ptr->tiles_vector[tile_index].type == TileType::PUSH_BACK)
		{
			//if player(obj) collides with a platforms
			if(CollisionWithTileDetected(world_ptr->tiles_vector[tile_index].x,world_ptr->tiles_vector[tile_index].y,
							   obj_x, obj_y, obj_width, obj_height) 
				)
			{
				//if player is not grounded and on top of a platform
				if(!grounded && (obj_y + obj_height - 1 <= world_ptr->tiles_vector[tile_index].y))
				{
					obj_vy = 0;
					obj_y = world_ptr->tiles_vector[tile_index].y - obj_height;
					grounded = true;
				}
					
				//if on the ground
				if(grounded)
				{
					//if player is on top of platform
					if(obj_y + obj_height - 1 <= world_ptr->tiles_vector[tile_index].y)
					{
						obj_vy = 0;
						obj_y = world_ptr->tiles_vector[tile_index].y - obj_height;
						
					}
					else
					{
						
						//if player is to the left of platform 
						if(obj_x + obj_width - 1 <= world_ptr->tiles_vector[tile_index].x + 20)
						{
							//PushLeft(obj_x,obj_vx,dt);
							obj_vx = 0;
							obj_x = world_ptr->tiles_vector[tile_index].x - obj_width;
						}
						//else if player is to the right of platform
						else if(obj_x + 1 >= world_ptr->tiles_vector[tile_index].x + 30 - 20)
						{
							//PushRight(obj_x,obj_vx,dt);
							obj_vx = 0;
							obj_x = world_ptr->tiles_vector[tile_index].x + 30;
						}
						else
						{
							
							PushUp(obj_y,obj_vy,dt);
							
						}
												
					}
				}
				//else if not grounded
				else
				{
					
					//std::cout << "\nin air.\n";
					
					/*
					std::cout << "player right: " << obj_x + obj_width - 1 << std::endl;
					std::cout << "player left: " << obj_x + 1 << std::endl;
					
					std::cout << "platform left:" << main_stage.collision_rect_array[i].rect.x << std::endl;
					std::cout << "platform right:" << main_stage.collision_rect_array[i].rect.x + main_stage.collision_rect_array[i].rect.width << std::endl;
					std::cout << "platform up:" << main_stage.collision_rect_array[i].rect.y  << std::endl;
					std::cout << "platform down:" << main_stage.collision_rect_array[i].rect.y + main_stage.collision_rect_array[i].rect.height << std::endl;
					*/
					
					//if player is to the left of platform 
					if(obj_x + obj_width - 1 <= world_ptr->tiles_vector[tile_index].x + 20)
					{
						//std::cout << "\nplayer below platform to the left. Push left.\n";
						//PushLeft(obj_x,obj_vx,dt);
						obj_vx = 0;
						obj_x = world_ptr->tiles_vector[tile_index].x - obj_width;
					}
					//else if player is to the right of platform
					else if(obj_x + 1 >= world_ptr->tiles_vector[tile_index].x + 30 - 20)
					{
						//std::cout << "\nplayer below platform to the right. Push right.\n";
						//PushRight(obj_x,obj_vx,dt);
						obj_vx = 0;
						obj_x = world_ptr->tiles_vector[tile_index].x + 30;
					}
					
					//if player is on top of platform
					if(obj_y + obj_height - 2 <= world_ptr->tiles_vector[tile_index].y + 10)
					{
						//std::cout << "\nplayer above platform. Push up.\n";
						//PushUp(obj_y,obj_vy,dt);
						obj_vy = 0;
						obj_y = world_ptr->tiles_vector[tile_index].y - obj_height;
						grounded = true;
					}
					else
					{
						//std::cout << "\nplayer below platform. Push down.\n";
						PushDown(obj_y,obj_vy,dt);
					}
						
					
				}
			}
		}

	}
	
}

static float level_bound_left_x = 0;
static float level_bound_right_x = 6600;
static float level_bound_up_y = 0;
static float level_bound_down_y = 6240 + 500;

static void CheckCollisionWithLevelBounds_MetroidVania(float& obj_x, float& obj_y, 
										float& obj_vx, float& obj_vy, 
										float& dt, 
										std::uint32_t& obj_width,
										std::uint32_t& obj_height)
{
	//if go to the right of the level bound
	if(obj_x + obj_width >= level_bound_right_x)
	{
		obj_x = level_bound_right_x - obj_width - 1;
	}
	//if go to the left of level bound
	else if(obj_x <= level_bound_left_x)
	{
		
		obj_x = level_bound_left_x + 1;
	}
	
	//if go above up bound
	if(obj_y + 2*obj_height <= level_bound_up_y)
	{
		//push back player 
		PushBack(obj_x, obj_y, 
			obj_vx, obj_vy, 
			dt);
		obj_y = level_bound_up_y + 1;
	}
	
	//if go below down bound
	else if(obj_y + obj_height >= level_bound_down_y)
	{
		//push back player 
		PushBack(obj_x, obj_y, 
			obj_vx, obj_vy, 
			dt);
		
		obj_y = level_bound_down_y - obj_height - 1;
	}
}

static float stage_bound_left_x = 0;
static float stage_bound_right_x = 640;
static float stage_bound_up_y = 0;
static float stage_bound_down_y = 360;

static void CheckCollisionWithLevelBounds_Versus(float& obj_x, float& obj_y, 
										float& obj_vx, float& obj_vy, 
										float& dt, 
										std::uint32_t& obj_width,
										std::uint32_t& obj_height)
{
	//if go to the right of the level bound
	if(obj_x + obj_width >= 640.0f)
	{
		obj_x = stage_bound_right_x - obj_width - 1;
	}
	//if go to the left of level bound
	else if(obj_x <= 0.0f)
	{
		
		obj_x = stage_bound_left_x + 1;
	}
	
	//if go above up bound
	if(obj_y + 2*obj_height <= 0.0f)
	{
		//push back player 
		PushBack(obj_x, obj_y, 
			obj_vx, obj_vy, 
			dt);
		obj_y = stage_bound_up_y + 1;
	}
	
	//if go below down bound
	else if(obj_y + obj_height >= 360.0f)
	{
		//push back player 
		PushBack(obj_x, obj_y, 
			obj_vx, obj_vy, 
			dt);
		
		obj_y = stage_bound_down_y - obj_height - 1;
	}
}

static float jump_factor = 70.0f;

void PhysicsSystem::Update_VersusMode(float& dt)
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
				
				float jumpVel = rigidBody.jump_speed*jump_factor;
								
				if(physics_type_comp.jump_count >= 1)
				{
					jumpVel = 0;
				}
				
				//if jumping
				if(jumpVel < 0)
				{					
					physics_type_comp.grounded = false;
					physics_type_comp.jump_count++;
					
					rigidBody.velocity.y += jumpVel*2.0f;
					
				}
				
				rigidBody.velocity.y += (gravity.force.y * dt);
				
				//move transform component by velocity of rigid body multiplied by time
				//std::cout << "In physics system, player rigid body velocity: " << rigidBody.velocity.x << std::endl;
				transform.position.x += rigidBody.velocity.x * dt;
				transform.position.y += rigidBody.velocity.y * dt;
				
				
				
				CheckCollisionWithPlatforms(transform.position.x, transform.position.y,
											rigidBody.velocity.x, rigidBody.velocity.y,
											dt,
											collisionBox.width, collisionBox.height,
											physics_type_comp.grounded);
				
				if(physics_type_comp.grounded)
				{
					physics_type_comp.jump_count = 0;
				}
				
				CheckCollisionWithLevelBounds_Versus(transform.position.x, transform.position.y,
											  rigidBody.velocity.x, rigidBody.velocity.y,
											  dt,
											  collisionBox.width, collisionBox.height);
				
				
				break;
			}
			default:{break;}
		}
		
	}
	
}

void PhysicsSystem::Update_MetroidVaniaMode(float& dt)
{
	//handle physics i.e. push back from tile collision and gravity
	//only handle tiles near player
		
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
				
				float jumpVel = rigidBody.jump_speed*jump_factor;
				
				bool jump = false;
				
				if(physics_type_comp.jump_count >= 1)
				{
					jumpVel = 0;
				}
				
				//if jumping
				if(jumpVel < 0)
				{					
					physics_type_comp.grounded = false;
					physics_type_comp.jump_count++;
					rigidBody.velocity.y += jumpVel*2.0f;
				}
				else if(jumpVel > 0)
				{
					physics_type_comp.grounded = false;
				}
				
				rigidBody.velocity.y += (gravity.force.y * dt);
				
				//move transform component by velocity of rigid body multiplied by time
				//std::cout << "In physics system, player rigid body velocity: " << rigidBody.velocity.x << std::endl;
				transform.position.x += 3*rigidBody.velocity.x * dt;
				transform.position.y += rigidBody.velocity.y * dt;
				
				//if world one is active i.e. a player is in world one
				if(world_one.in_active_use)
				{
					//if the collision box of player is in world one
					if(collisionBox.world_id == 0)
					{
						
						CheckCollisionWithTiles(transform.position.x, transform.position.y,
											rigidBody.velocity.x, rigidBody.velocity.y,
											dt,
											collisionBox.width, collisionBox.height,
											physics_type_comp.grounded,
											&world_one);
					}
					
				}
				
				
				if(physics_type_comp.grounded)
				{
					physics_type_comp.jump_count = 0;
				}
				
				CheckCollisionWithLevelBounds_MetroidVania(transform.position.x, transform.position.y,
											  rigidBody.velocity.x, rigidBody.velocity.y,
											  dt,
											  collisionBox.width, collisionBox.height);
				
				
				break;
			}
			default:{break;}
		}
		
	}
}



