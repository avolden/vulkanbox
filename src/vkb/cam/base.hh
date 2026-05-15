#pragma once

#include "../math/mat4.hh"
#include "../math/vec4.hh"

namespace vkb::cam
{
	class base
	{
	public:
		mat4 view_mat() const;
		mat4 rot_mat() const;

		vec4 pos() const;
		vec4 up() const;
		vec4 fwd() const;

	protected:
		base() = default;

		mat4 view_mat_;
		mat4 rot_mat_;

		vec4 pos_;
		vec4 up_;
		vec4 fwd_;
	};
}