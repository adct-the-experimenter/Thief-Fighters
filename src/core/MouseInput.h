#ifndef MOUSE_INPUT_H
#define MOUSE_INPUT_H

#include <cstdint>
#include <vector>


//assuming only 1 mouse is needed at this time	
									
class MouseInput
{
public:
	
												
	struct MouseInfo {
		
		float x;
		float y;
		
		bool clicked;
	};
	
	MouseInfo mouse_info;
	
	void Init(std::uint8_t num_players)
	{
		mouse_info.x = 0; mouse_info.y = 0;
		mouse_info.clicked = false;
	};
	
	void Reset()
	{
		mouse_info.x = 0; mouse_info.y = 0;
		mouse_info.clicked = false;
	};
};

#endif
