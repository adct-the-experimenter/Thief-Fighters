#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include "misc/globalvariables.h"
#include <cstdint>

										
class KeyboardInput
{
public:
	
	char key;
	
	bool backspace_pressed;
	
	bool enter_pressed;
	
	bool valid_key;
	
	void Reset()
	{
		key = 0;
		backspace_pressed = false;
		enter_pressed = false;
		valid_key = false;
	};
};

#endif
