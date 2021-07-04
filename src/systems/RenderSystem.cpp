#include "RenderSystem.h"

#include "core/system.h"

#include "core/coordinator.h"

#include <iostream>

#include <cmath>

extern Coordinator gCoordinator;

#include "misc/level_maps.h"

#include "misc/char_media.h"

void RenderSystem::Init(CustomCamera* camera)
{
	m_camera_ptr = camera;
        
}

void RenderSystem::Init_MetroidVaniaMode(CameraManager* camera_manager_ptr)
{
	m_camera_manager_ptr = camera_manager_ptr;
}

static bool IsObjectInCameraView(float& posX, float& posY, Rectangle& camera_rect)
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
	//render player only if in camera. determined by world system
			
	if(this->m_camera_ptr)
	{			
		
		//for every entity
		for (auto const& entity : mEntities)
		{
			auto& render_comp = gCoordinator.GetComponent<RenderModelComponent>(entity);
			auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
			
			
			//if renderable object is within camera bounds.
			if(render_comp.render)
			{
				//adjust render position relative to camera position
				Vector2 adjusted_pos;
				adjusted_pos.x = transform.position.x - m_camera_ptr->GetCameraRectPointer()->x;
				adjusted_pos.y = transform.position.y - m_camera_ptr->GetCameraRectPointer()->y;
				
				//render texuture according to frame, adjusted camera composition, tint
				DrawTextureRec(character_sheet_textures[render_comp.char_texture_index], render_comp.frame_rect, adjusted_pos, render_comp.tint);
			}
		}
					
	}
		
	
}

void RenderSystem::Update_MetroidVaniaMode()
{
	if(m_camera_manager_ptr)
	{
		//for every entity
		for (auto const& entity : mEntities)
		{
			auto& render_comp = gCoordinator.GetComponent<RenderModelComponent>(entity);
			auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
			
			for(size_t i = 0; i < m_camera_manager_ptr->lead_cameras.size(); i++)
			{
				
				//if renderable object is within camera bounds.
				if(render_comp.render)
				{
					
					if(m_camera_manager_ptr->lead_cameras[i].GetCameraActiveStatus())
					{
						Rectangle* camera_ptr = m_camera_manager_ptr->lead_cameras[i].GetCameraRectPointer();
						
						//adjust render position relative to camera position
						Vector2 adjusted_pos;
						
						adjusted_pos.x = transform.position.x - camera_ptr->x;
						adjusted_pos.y = transform.position.y - camera_ptr->y;
						
						//render texuture according to frame, adjusted camera composition, tint
						DrawTextureRec(character_sheet_textures[render_comp.char_texture_index], render_comp.frame_rect, adjusted_pos, render_comp.tint);
					}
					
				}
			}
			
			
		}
	}
}

void RenderSystem::Update_MetroidVaniaMode_Editor()
{
	if(m_camera_manager_ptr)
	{
		//for every entity
		for (auto const& entity : mEntities)
		{
			auto& render_comp = gCoordinator.GetComponent<RenderModelComponent>(entity);
			auto& transform = gCoordinator.GetComponent<Transform2D>(entity);
			
			for(size_t i = 0; i < m_camera_manager_ptr->lead_cameras.size(); i++)
			{
				
				//if renderable object is within camera bounds.
				if(render_comp.render)
				{
					
					if(m_camera_manager_ptr->lead_cameras[i].GetCameraActiveStatus())
					{
						Rectangle* camera_ptr = m_camera_manager_ptr->lead_cameras[i].GetCameraRectPointer();
						
						//adjust render position relative to camera position
						Vector2 adjusted_pos;
						
						adjusted_pos.x = transform.position.x - camera_ptr->x + 250;
						adjusted_pos.y = transform.position.y - camera_ptr->y;
						
						//render texuture according to frame, adjusted camera composition, tint
						DrawTextureRec(character_sheet_textures[render_comp.char_texture_index], render_comp.frame_rect, adjusted_pos, render_comp.tint);
					}
					
				}
			}
			
			
		}
	}
}
