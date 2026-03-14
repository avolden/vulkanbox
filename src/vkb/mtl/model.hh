#pragma once

#include <stdint.h>

namespace MTL
{
	class Buffer;
}

namespace vkb::mtl
{
	struct model
	{
		model(void* verts, uint32_t verts_size, uint16_t* idcs, uint32_t idcs_size);
		~model();

		MTL::Buffer* vertex_buf {nullptr};
		MTL::Buffer* index_buf {nullptr};

		uint32_t idcs_count {0};
	};
}