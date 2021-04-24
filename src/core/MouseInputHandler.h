#ifndef MOUSE_INPUT_HANDLER_H
#define MOUSE_INPUT_HANDLER_H

#include "core/MouseInput.h"

enum class MouseInputMode {ACTIVE_ON_CLICK = 0, ALWAYS_ACTIVE};

class MouseInputHandler
{
public:
	
	void Init();
	
	void Update(MouseInput* input);
	
private:
		
	void SetMouseInfo(MouseInput& input_info);
	
	MouseInputMode m_mode;
};

#endif
