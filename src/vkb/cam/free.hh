#pragma once

#include "../math/mat4.hh"

#include "../math/vec4.hh"

#include "base.hh"

namespace vkb
{
	class input_system;
	class window;

	namespace ui
	{
		class context;
	}
}

namespace vkb::cam
{
	class free : public base
	{
		friend ui::context;

	public:
		free(input_system& is, window& win);

		void update(double dt);

	private:
		input_system& is_;
		window&       win_;

		float yaw_ {0.f};
		float pitch_ {0.f};
	};
}