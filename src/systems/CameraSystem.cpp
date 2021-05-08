#include "CameraSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void CameraSystem::Init(CustomCamera* camera, std::uint8_t num_players)
{
	m_camera_ptr = camera;
	m_num_players = num_players;
}

void CameraSystem::Update(float& scale)
{
	std::uint8_t current_player = 0;
	
	
	
	for (auto const& entity : mEntities)
	{
		
		auto& transform = gCoordinator.GetComponent <Transform2D> (entity);
		auto& collisionBox = gCoordinator.GetComponent <CollisionBox> (entity);
		auto& player = gCoordinator.GetComponent <Player> (entity);
		
		
		//center camera to follow all players on screen
		
		Rectangle* camera_rect_ptr = m_camera_ptr->GetCameraRectPointer();
		
		//if only 1 player is playing
		if(m_num_players == 1)
		{
			//center camera over the only player
			camera_rect_ptr->x = ( transform.position.x + (collisionBox.width / 2) ) - (camera_rect_ptr->width / 2);
			camera_rect_ptr->y = ( transform.position.y + (collisionBox.height / 2) ) - (camera_rect_ptr->height / 2);
			
			
		}
		//else if more than 1 player is playing
		else if(m_num_players > 1)
		{
			//center camera to point between player on the farthest left side
			//and player on the farthest right side
			
		}
		
		
		
		//get level bounds
		std::uint16_t& boundDown = m_camera_ptr->GetDownLevelBound();
		std::uint16_t& boundUp = m_camera_ptr->GetUpLevelBound();
		std::uint16_t& boundLeft = m_camera_ptr->GetLeftLevelBound();
		std::uint16_t& boundRight = m_camera_ptr->GetRightLevelBound();
		
		//Keep the camera in bounds
		if( camera_rect_ptr->x < 0 )
		{ 
			camera_rect_ptr->x = 0;
		}
		
		if( camera_rect_ptr->y < 0 )
		{
			camera_rect_ptr->y = 0;
		}
		
		if( camera_rect_ptr->x > boundRight - camera_rect_ptr->width )
		{
			camera_rect_ptr->x = boundRight - camera_rect_ptr->width;
		}
		
		if( camera_rect_ptr->y > boundDown - camera_rect_ptr->height )
		{
			camera_rect_ptr->y = boundDown - camera_rect_ptr->height;
		}
		
		//std::cout << "\nIn Camera System\n";
		//std::cout << "camera position: " << camera_rect_ptr->x << " , " << camera_rect_ptr->y << std::endl;
		
	}
	 
	
}

