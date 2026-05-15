#pragma once

#include "vec4.hh"

namespace vkb
{
	struct ray
	{
		vec4  o;
		vec4  dir;
		float dist;
	};
}