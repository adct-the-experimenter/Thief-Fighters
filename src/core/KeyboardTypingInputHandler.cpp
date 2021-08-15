#include "KeyboardTypingInputHandler.h"

#include "raylib.h"
#include <iostream>


void KeyboardTypingInputHandler::Init()
{
	
}

void KeyboardTypingInputHandler::Update(KeyboardInput* input)
{
	input->Reset();
	KeyboardTypingInputHandler::SetKeyboardTypingInfo(*input);
}

void KeyboardTypingInputHandler::SetKeyboardTypingInfo(KeyboardInput& input_info)
{
	
	// Get char pressed (unicode character) on the queue
	int key = GetCharPressed();

	// Check if more characters have been pressed on the same frame
	while(key > 0)
	{
		// NOTE: Only allow keys in range [32..125]
		if ( (key >= 32) && (key <= 125) )
		{
			input_info.valid_key = true;
			input_info.key = (char)key;
		}
		
		key = GetCharPressed();  // Check next character in the queue
	}

	if (IsKeyPressed(KEY_BACKSPACE))
	{
		input_info.backspace_pressed = true;
	}
	else
	{
		input_info.backspace_pressed = false;
	}
	
	if( IsKeyPressed(KEY_ENTER) )
	{
		input_info.enter_pressed = true;
	}
	else
	{
		input_info.enter_pressed = false;
	}
	
}
