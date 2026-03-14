#include "model.hh"
#include "instance.hh"
#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLDevice.hpp>
#include <Metal/MTLResource.hpp>

namespace vkb::mtl
{
	model::model(void* verts, uint32_t verts_size, uint16_t* idcs, uint32_t idcs_size)
	{
		instance& inst = instance::get();
		vertex_buf = inst.get_device()->newBuffer(verts, verts_size,
		                                          MTL::ResourceStorageModeShared);
		index_buf =
			inst.get_device()->newBuffer(idcs, idcs_size, MTL::ResourceStorageModeShared);
		idcs_count = idcs_size / sizeof(uint16_t);
	}

	model::~model()
	{
		if (vertex_buf)
			vertex_buf->release();
		if (index_buf)
			index_buf->release();
	}
}