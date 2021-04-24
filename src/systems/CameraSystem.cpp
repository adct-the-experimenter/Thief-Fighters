#include "CameraSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

extern Coordinator gCoordinator;


void CameraSystem::Init(CustomCamera* camera,
				std::uint16_t screenWidth, std::uint16_t screenHeight)
{
	m_camera_ptr = camera;
	
	*m_camera_ptr->GetPointerToCamera() = { { 0.2f, 0.4f, 0.2f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
	//m_camera_ptr->camera_rect.width = screenWidth;
	//m_camera_ptr->camera_rect.height = screenHeight;
	//SetCameraMode(camera->GetReferenceToCamera(), CAMERA_FREE);     // Set camera mode
	
	
}

void CameraSystem::Update()
{
	std::uint8_t current_player = 0;
	
	for (auto const& entity : mEntities)
	{
		
		auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
		auto& player = gCoordinator.GetComponent<Player>(entity);
		
		
		//center camera to follow both players
		
		//put camera at halfway point of distance vector made from  2 players positions
		//zoom based on distance between 2 players.
		
		
	}
	
	// Update camera
	//UpdateCamera(m_camera_ptr->GetPointerToCamera());      
	
}

