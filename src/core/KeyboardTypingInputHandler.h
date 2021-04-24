#ifndef KEYBOARD_INPUT_HANDLER_H
#define KEYBOARD_INPUT_HANDLER_H

#include "core/KeyboardInput.h"
#include "misc/globalvariables.h" //for SDL2 gamepad and data dir path string

class KeyboardTypingInputHandler
{
public:
	
	void Init();
	
	void Update(KeyboardInput* input);
	
private:
		
	void SetKeyboardTypingInfo(KeyboardInput& input_info);
	
	
};

#endif
