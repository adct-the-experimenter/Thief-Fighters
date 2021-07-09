#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <cstdint>
#include "core/system.h"

#include <array>
#include <vector>
#include "misc/camera.h"


class RenderSystem : public System
{
public:
	void Init(CustomCamera* camera);

	void Update();
	
	void Init_MetroidVaniaMode(CameraManager* camera_manager_ptr);
	void Update_MetroidVaniaMode();
	
	void Update_MetroidVaniaMode_Editor();
		
private:

	CustomCamera* m_camera_ptr;
	CameraManager* m_camera_manager_ptr;
};

#endif
