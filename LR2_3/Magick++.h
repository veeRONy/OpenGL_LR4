#pragma once

#include <Magick++/Include.h>
#include <Magick++/Functions.h>
#include <Magick++/Image.h>
#include <Magick++/Pixels.h>
#include <Magick++/ResourceLimits.h>
#include <Magick++/STL.h>

// Don't leak our definition of the 'restrict' keyword. 'restrict' is a valid
// identifier in C++, and leaking it could cause extraneous build failures.
#ifdef restrict
#undef restrict
#endif

