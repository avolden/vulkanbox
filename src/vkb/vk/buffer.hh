#pragma once

#include "vma/vma.hh"
#include <volk/volk.h>

namespace vkb::vk
{
	struct buffer
	{
		VkBuffer      buffer {nullptr};
		VmaAllocation memory {nullptr};
	};
}