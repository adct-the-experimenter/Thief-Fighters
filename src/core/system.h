
#ifndef SYSTEM_H
#define SYSTEM_H

#include <set>
#include "core/entity.h"

class System
{
public:
	std::set<Entity> mEntities;
};

#endif
