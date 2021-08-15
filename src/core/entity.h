//    MIT License

//    Copyright (c) 2020 Austin Morlan

//    Permission is hereby granted, free of charge, to any person obtaining a copy
//    of this software and associated documentation files (the "Software"), to deal
//    in the Software without restriction, including without limitation the rights
//    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//    copies of the Software, and to permit persons to whom the Software is furnished
//    to do so, subject to the following conditions:

//    The above copyright notice and this permission notice (including the next
//    paragraph) shall be included in all copies or substantial portions of the
//    Software.

//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
//    OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
//    OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
