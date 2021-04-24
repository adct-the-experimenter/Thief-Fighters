#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <bitset>
#include "core/component.h"

// A simple type alias
using Entity = std::uint32_t;

// Used to define the size of arrays later on
const Entity MAX_ENTITIES = 34;

// A simple type alias.
// Bitset used to identify which components an entity has
using Signature = std::bitset<MAX_COMPONENTS>;

#endif
