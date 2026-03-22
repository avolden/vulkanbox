#include "coordinates.hh"
#include "vkb/cam/base.hh"
#include "vkb/math/mat4.hh"
#include "vkb/math/vec2.hh"
#include "vkb/mtl/instance.hh"

#include <Foundation/NSString.hpp>
#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLDepthStencil.hpp>
#include <Metal/MTLDevice.hpp>
#include <Metal/MTLLibrary.hpp>
#include <Metal/MTLPixelFormat.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <Metal/MTLRenderPipeline.hpp>

namespace vkb::mtl
{
	namespace
	{
		struct alignas(16) mvp
		{
			mat4 view;
			mat4 proj;
			vec2 translate;
		};
	}

	coordinates::coordinates()
	{
		instance& inst = instance::get();

		lib_ = inst.get_device()->newLibrary(
			NS::String::string("res/shaders/metal/coordinates.metallib",
		                       NS::ASCIIStringEncoding),
			nullptr);

		MTL::Function* vertex =
			lib_->newFunction(NS::String::string("v_main", NS::ASCIIStringEncoding));
		MTL::Function* fragment =
			lib_->newFunction(NS::String::string("f_main", NS::ASCIIStringEncoding));

		MTL::RenderPipelineDescriptor* desc =
			MTL::RenderPipelineDescriptor::alloc()->init();
		desc->setLabel(NS::String::string("Coordinates", NS::ASCIIStringEncoding));
		desc->setVertexFunction(vertex);
		desc->setFragmentFunction(fragment);
		// TODO either hardcode or retrieve from current surface drawn onto
		desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
		desc->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);

		NS::Error* err;
		pso_ = inst.get_device()->newRenderPipelineState(desc, &err);

		for (uint32_t i {0}; i < 2; ++i)
			mvp_[i] =
				inst.get_device()->newBuffer(sizeof(mvp), MTL::ResourceStorageModeShared);
	}

	coordinates::~coordinates()
	{
		// TODO release resources
	}

	void coordinates::prepare_draw(uint32_t cur_img, cam::base const& cam,
	                               mat4 const& proj, vec2 const& translate)
	{
		MTL::Buffer* buf = mvp_[cur_img];

		mvp mvp;

		mvp.view = cam.rot_mat();
		mvp.proj = proj;
		mvp.translate = translate;

		memcpy(buf->contents(), &mvp, sizeof(mvp));
	}

	void coordinates::draw(uint32_t cur_img, MTL::RenderCommandEncoder* cmd)
	{
		instance& inst = instance::get();

		MTL::DepthStencilDescriptor* depth_desc =
			MTL::DepthStencilDescriptor::alloc()->init();
		depth_desc->setDepthWriteEnabled(false);
		depth_desc->setDepthCompareFunction(MTL::CompareFunctionAlways);
		MTL::DepthStencilState* depth_state =
			inst.get_device()->newDepthStencilState(depth_desc);
		cmd->setDepthStencilState(depth_state);

		depth_state->release();
		depth_desc->release();

		cmd->setRenderPipelineState(pso_);
		cmd->setVertexBuffer(mvp_[cur_img], 0, 0);
		cmd->setCullMode(MTL::CullModeNone);
		cmd->drawPrimitives(MTL::PrimitiveTypeLine, NS::Integer(0), 6);
	}
}