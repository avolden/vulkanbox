#pragma once

#include "../../math/vec2.hh"
#include "../../math/vec4.hh"

#include "../vma/vma.hh"
#include <volk/volk.h>

#include "../buffer.hh"
#include <array.hh>

namespace vkb::vk
{
	struct model
	{
		struct vert
		{
			vec4 pos;
			vec4 col;
			vec2 uv;
		};

		static VkVertexInputBindingDescription                 binding_desc();
		static mc::array<VkVertexInputAttributeDescription, 3> attribute_descs();

		buffer vertex_;
		buffer index_;

		uint32_t idcs_size_ {0};
	};
}