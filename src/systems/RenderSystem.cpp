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

