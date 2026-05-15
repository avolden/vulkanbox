#pragma once

#include "vec4.hh"

namespace vkb
{
	struct ray;

	struct plane
	{
		vec4  n;
		vec4  o;
		float size;

		bool intersects(ray const& r) const;
	};
}