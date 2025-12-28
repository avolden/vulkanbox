#pragma once

#include "../vma/vma.hh"
#include <volk/volk.h>

#include "../image.hh"

namespace vkb::vk
{
	struct texture
	{
		uint32_t    mip_lvl {0};
		image       img;
		VkImageView img_view {nullptr};
		VkSampler   sampler {nullptr};
	};
}