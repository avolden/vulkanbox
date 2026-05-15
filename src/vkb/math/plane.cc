#include "plane.hh"

#include "ray.hh"

namespace vkb
{
	bool plane::intersects(ray const& r) const
	{
		if (r.dir.dot3(n) > 0)
			return false;

		float dist = (o - r.o).dot3(n) / (r.dir.dot3(n));

		if (dist > r.dist)
			return false;

		vec4 point = r.o + r.dir * dist;

		if ((point - o).sq_len() < size * size)
			return true;

		return false;
	}
}