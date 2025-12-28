#pragma once

#include "vma/vma.hh"
#include <volk/volk.h>

namespace vkb::vk
{
	struct image
	{
		VkImage       image {nullptr};
		VmaAllocation memory {nullptr};
	};
}