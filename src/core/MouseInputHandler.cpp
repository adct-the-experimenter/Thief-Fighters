#include "MouseInputHandler.h"

#include "raylib.h"
#include <iostream>


void MouseInputHandler::Init()
{
	m_mode = MouseInputMode::ACTIVE_ON_CLICK;
}

void MouseInputHandler::Update(MouseInput* input)
{
	input->Reset();
	MouseInputHandler::SetMouseInfo(*input);
}

void MouseInputHandler::SetMouseInfo(MouseInput& input_info)
{
	
	
	switch(m_mode)
	{
		case MouseInputMode::ACTIVE_ON_CLICK:
		{
			input_info.mouse_info.clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
			
			if(input_info.mouse_info.clicked)
			{
				input_info.mouse_info.x = GetMousePosition().x;
				input_info.mouse_info.y = GetMousePosition().y;
			}
			
			break;
		}
		case MouseInputMode::ALWAYS_ACTIVE:
		{
			input_info.mouse_info.clicked = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
			input_info.mouse_info.x = GetMousePosition().x;
			input_info.mouse_info.y = GetMousePosition().y;
			
			break;
		}
		default:{break;}
	}
}
