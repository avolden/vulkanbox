#pragma once

#include <string_view.hh>

#include <stdint.h>

namespace MTL
{
	class Texture;
}

namespace vkb::mtl
{
	struct texture
	{
		texture(mc::string_view path);
		~texture();

		uint32_t width {0};
		uint32_t height {0};

		MTL::Texture* tex;
	};
}