#include "RenderSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

#include <cmath>

extern Coordinator gCoordinator;

#include "misc/level_maps.h"


void RenderSystem::Init(CustomCamera* camera)
{
	m_camera_ptr = camera;
        
}

bool IsObjectInCameraView(float& posX, float& posY, Rectangle& camera_rect)
{
	//half the width and height because the camera is centered on a player.
	
	if(posX < camera_rect.x)
	{
		return false;
	}
	else if(posX > camera_rect.x + (camera_rect.width / 2))
	{
		return false;
	}
	
	if(posY < camera_rect.y)
	{
		return false;
	}
	else if(posY > camera_rect.y + (camera_rect.height / 2))
	{
		return false;
	}
	
	return true;
}



void RenderSystem::Update()
{
			
	if(this->m_camera_ptr)
	{			
		
		//for every entity
		for (auto const& entity : mEntities)
		{
			auto& render_comp = gCoordinator.GetComponent<RenderModelComponent>(entity);
			auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
			
			//bool renderObjectBool = IsObjectInCameraView(transform.position.x,transform.position.y,m_camera_ptr->camera_rect);
			bool renderObjectBool = true;
			
			//if renderable object is within camera bounds.
			if(renderObjectBool)
			{
				//DrawModelEx( render_comp.model, transform.position, (Vector3){ 1.0f, 0.0f, 0.0f }, -90.0f, (Vector3){ 1.0f, 1.0f, 1.0f }, WHITE);
				
				//DrawTextureV(*render_comp.texture_ptr, transform.position, RAYWHITE);
				DrawTextureRec(*render_comp.texture_ptr, render_comp.frame_rect, transform.position, RAYWHITE);
			}
		}
		
		
					
	}
		
	
}

