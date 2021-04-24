
#ifndef INPUT_REACTOR_SYSTEM_H
#define INPUT_REACTOR_SYSTEM_H

#include "core/system.h"

#include "core/ControllerInput.h"

class InputReactorSystem : public System
{
public:

	void Update(ControllerInput& input);
};

#endif
