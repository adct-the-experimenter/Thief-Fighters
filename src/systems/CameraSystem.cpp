#include "CameraSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>
#include <cmath>

extern Coordinator gCoordinator;


void CameraSystem::Init(CustomCamera* camera, std::uint8_t num_players)
{
	m_camera_ptr = camera;
	m_num_players = num_players;
	
	zoom_time_counter = 0;

}

void CameraSystem::Init_MetroidVaniaMode(CameraManager* camera_m_ptr, std::uint8_t num_players, 
										std::uint32_t gameScreenWidth, std::uint32_t gameScreenHeight)
{
	m_camera_manager_ptr = camera_m_ptr;
	m_num_players = num_players;
	
	//initialize cameras based on number of players
	switch(num_players)
	{
		case 1:
		{
			//1 camera 
			m_camera_manager_ptr->lead_cameras[0].Init(gameScreenWidth,gameScreenHeight);
			m_camera_manager_ptr->lead_cameras[0].SetCameraActiveStatus(true);
			break;
		}
		case 2:
		{
			//2 cameras, top and bottom
			m_camera_manager_ptr->lead_cameras[0].Init(gameScreenWidth ,gameScreenHeight / 2);
			m_camera_manager_ptr->lead_cameras[0].SetCameraActiveStatus(true);
			
			m_camera_manager_ptr->lead_cameras[1].Init(gameScreenWidth ,gameScreenHeight / 2);
			m_camera_manager_ptr->lead_cameras[1].SetCameraActiveStatus(true);
			break;
		}
		case 3:
		{ 
			//3 cameras in upside down triangle formation
			m_camera_manager_ptr->lead_cameras[0].Init(gameScreenWidth / 2,gameScreenHeight / 2);
			m_camera_manager_ptr->lead_cameras[0].SetCameraActiveStatus(true);
			
			m_camera_manager_ptr->lead_cameras[1].Init(gameScreenWidth / 2,gameScreenHeight / 2);
			m_camera_manager_ptr->lead_cameras[1].SetCameraActiveStatus(true);
			
			m_camera_manager_ptr->lead_cameras[2].Init(gameScreenWidth / 2,gameScreenHeight / 2);
			m_camera_manager_ptr->lead_cameras[2].SetCameraActiveStatus(true);
			
			break;
		}
	}
	
	if(num_players >= 4)
	{
		//4 equal size cameras at top and bottom
		for(size_t i = 0; i < m_camera_manager_ptr->lead_cameras.size(); i++)
		{
			m_camera_manager_ptr->lead_cameras[i].Init(gameScreenWidth / 2,gameScreenHeight / 2);
			m_camera_manager_ptr->lead_cameras[i].SetCameraActiveStatus(true);
			//m_camera_manager_ptr.lead_cameras[0].SetLevelBounds(0,gameScreenWidth / 2,0,gameScreenHeight / 2);
		}
		
	}
}

void CameraSystem::Update_MetroidVaniaMode()
{
	//for 4 or less players
	//number of rectangle clips of world = number of players
	//follow camera leads
	
	
	//for 5 or more players, handle only 4 rectangle clips of world
	//follow camera leads
	//implement bound check on player to prevent non-camera lead player from moving out of
	//camera bounds
	
	//Change camera configurations depending on how close camera leads are to each other
	//if one camera lead is 3/4 inside another camera lead's camera join together.
	//else if one camera lead is 3/4 outside another camera lead's camera split.
		
	//for metroidvania game mode
	for (auto const& entity : mEntities)
	{
		auto& player = gCoordinator.GetComponent<Player>(entity);
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& collisionBox = gCoordinator.GetComponent<CollisionBox>(entity);
	
		if(player.camera_lead)
		{
			Rectangle* camera_rect = m_camera_manager_ptr->lead_cameras[ player.camera_num_leader ].GetCameraRectPointer();
			
			//center camera to player
			camera_rect->x = transform.position.x - (camera_rect->width / 2);
			camera_rect->y = transform.position.y - (camera_rect->height / 2);
			
			//level bounds
			if(camera_rect->x < 0){camera_rect->x = 0;}
			if(camera_rect->y < 0){camera_rect->y = 0;}
			
		}
	}
}

void CameraSystem::update_timer(float& dt){zoom_time_counter += dt;}
