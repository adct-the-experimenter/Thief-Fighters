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

void CameraSystem::Init_MetroidVaniaMode(CameraManager* camera_m_ptr, std::uint8_t num_players)
{
	m_camera_manager_ptr = camera_m_ptr;
	m_num_players = num_players;
}

void CameraSystem::Update_MetroidVaniaMode()
{
	//for metroidvania game mode
	
	//for 4 or less players
	//number of rectangle clips of world = number of players
	//follow camera leads
	
	
	//for 5 or more players, handle only 4 rectangle clips of world
	//follow camera leads
	//implement bound check on player to prevent non-camera lead player from moving out of
	//camera bounds
	
}

void CameraSystem::update_timer(float& dt){zoom_time_counter += dt;}
