#include "base.hh"

namespace vkb::cam
{
	mat4 base::view_mat() const
	{
		return view_mat_;
	}

	mat4 base::rot_mat() const
	{
		return rot_mat_;
	}

	vec4 base::pos() const
	{
		return pos_;
	}

	vec4 base::up() const
	{
		return up_;
	}

	vec4 base::fwd() const
	{
		return fwd_;
	}
}