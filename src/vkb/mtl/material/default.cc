#include "default.hh"

#include "../../math/mat4.hh"
#include "../../math/vec4.hh"

#include "../instance.hh"
#include "../model.hh"
#include "../texture.hh"
#include <Foundation/NSError.hpp>
#include <Foundation/NSString.hpp>
#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLDepthStencil.hpp>
#include <Metal/MTLDevice.hpp>
#include <Metal/MTLLibrary.hpp>
#include <Metal/MTLPixelFormat.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <Metal/MTLRenderPipeline.hpp>
#include <Metal/MTLResource.hpp>

namespace vkb::mtl
{
	triangle::triangle()
	{
		vec4 tri[6] {
			{-3.f, -1.f, -3.0f, 1.0f},
            {1.f,  0.f,  0.0f,  1.0f},
            {3.f,  -1.f, -3.0f, 1.0f},
			{0.f,  1.f,  0.0f,  1.0f},
            {3.0f, -1.f, 3.0f,  1.0f},
            {0.f,  0.f,  1.0f,  1.0f}
        };

		instance& inst = instance::get();

		model_ = inst.get_device()->newBuffer(tri, sizeof(tri),
		                                      MTL::ResourceStorageModeShared);

		lib_ = inst.get_device()->newLibrary(
			NS::String::string("res/shaders/metal/default.metallib",
		                       NS::ASCIIStringEncoding),
			nullptr);

		MTL::Function* vertex = lib_->newFunction(
			NS::String::string("vertexFunction", NS::ASCIIStringEncoding));
		MTL::Function* fragment = lib_->newFunction(
			NS::String::string("fragmentFunction", NS::ASCIIStringEncoding));

		MTL::RenderPipelineDescriptor* desc =
			MTL::RenderPipelineDescriptor::alloc()->init();
		desc->setLabel(NS::String::string("Triangle", NS::ASCIIStringEncoding));
		desc->setVertexFunction(vertex);
		desc->setFragmentFunction(fragment);
		// TODO either hardcode or retrieve from current surface drawn onto
		desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
		desc->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);

		NS::Error* err;
		pso_ = inst.get_device()->newRenderPipelineState(desc, &err);

		desc->release();
		vertex->release();
		fragment->release();

		for (uint32_t i {0}; i < 2; ++i)
			vp_[i] = inst.get_device()->newBuffer(sizeof(mat4) * 2,
			                                      MTL::ResourceStorageModeShared);
	}

	triangle::~triangle()
	{
		// TODO release resources
	}

	void triangle::prepare_draw(uint32_t cur_img, mat4 const& view, mat4 const& proj)
	{
		MTL::Buffer* buf = vp_[cur_img];

		struct vp_struct
		{
			mat4 view;
			mat4 proj;
		} vp;

		vp.view = view;
		vp.proj = proj;

		memcpy(buf->contents(), &vp, sizeof(vp));
	}

	void triangle::draw(model const& mod, texture const& tex, uint32_t cur_img,
	                    MTL::RenderCommandEncoder* cmd)
	{
		instance& inst = instance::get();

		MTL::DepthStencilDescriptor* depth_desc =
			MTL::DepthStencilDescriptor::alloc()->init();
		depth_desc->setDepthWriteEnabled(true);
		depth_desc->setDepthCompareFunction(MTL::CompareFunctionLess);
		MTL::DepthStencilState* depth_state =
			inst.get_device()->newDepthStencilState(depth_desc);
		cmd->setDepthStencilState(depth_state);

		depth_state->release();
		depth_desc->release();

		cmd->setRenderPipelineState(pso_);
		cmd->setVertexBuffer(mod.vertex_buf, 0, 0);
		cmd->setVertexBuffer(mod.index_buf, 0, 1);
		cmd->setVertexBuffer(vp_[cur_img], 0, 2);
		cmd->setFragmentTexture(tex.tex, 0);
		cmd->setCullMode(MTL::CullModeNone);
		cmd->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::Integer(0), mod.idcs_count);
	}
}