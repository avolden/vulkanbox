#include "context.hh"

#include "../math/trig.hh"
#include "instance.hh"
#include "surface.hh"
#include <Metal/MTLCommandBuffer.hpp>
#include <Metal/MTLCommandQueue.hpp>
#include <Metal/MTLDepthStencil.hpp>
#include <Metal/MTLDevice.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <Metal/MTLRenderPass.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

namespace vkb::mtl
{
	context::context(surface& surface)
	: surface_ {surface}
	{
		create_depth_texture();

		auto [w, h] = surface_.get_size();
		proj_ = mat4::persp_proj(0.1f, 100.f, w / (float)h, rad(70));
	}

	context::~context() {}

	bool context::created() const
	{
		return true;
	}

	bool context::prepare_draw()
	{
		instance& inst = instance::get();

		bool need_resize = false;
		if (surface_.need_resize())
		{
			surface_.resize();
			depth_tex_->release();
			create_depth_texture();

			auto [w, h] = surface_.get_size();
			proj_ = mat4::persp_proj(0.1f, 100.f, w / (float)h, rad(70));

			need_resize = true;
		}
		current_drawable_ = surface_.get_drawable();

		current_command_buffer_ = inst.get_queue()->commandBuffer();
		MTL::RenderPassDescriptor* desc = MTL::RenderPassDescriptor::alloc()->init();
		MTL::RenderPassColorAttachmentDescriptor* color =
			desc->colorAttachments()->object(0);
		color->setTexture(current_drawable_->texture());
		color->setLoadAction(MTL::LoadActionClear);
		desc->depthAttachment()->setTexture(depth_tex_);
		desc->depthAttachment()->setClearDepth(1.0);
		desc->depthAttachment()->setLoadAction(MTL::LoadActionClear);
		desc->depthAttachment()->setStoreAction(MTL::StoreActionDontCare);
		static float col = 0.1f;
		// static bool  reverse = false;
		// if (reverse)
		// {
		// 	col -= 0.005f;
		// 	if (col < 0.1f)
		// 	{
		// 		reverse = false;
		// 		col = 0.1f;
		// 	}
		// }
		// else
		// {
		// 	col += 0.005f;
		// 	if (col > 0.9f)
		// 	{
		// 		reverse = true;
		// 		col = 0.9f;
		// 	}
		// }
		color->setClearColor(MTL::ClearColor(col, col, col, 1.0));
		color->setStoreAction(MTL::StoreActionStore);

		MTL::DepthStencilDescriptor* depth_desc =
			MTL::DepthStencilDescriptor::alloc()->init();
		depth_desc->setDepthWriteEnabled(true);
		depth_desc->setDepthCompareFunction(MTL::CompareFunctionLess);
		MTL::DepthStencilState* depth_state =
			inst.get_device()->newDepthStencilState(depth_desc);

		depth_desc->release();

		encoder_ = current_command_buffer_->renderCommandEncoder(desc);
		encoder_->setDepthStencilState(depth_state);

		depth_state->release();
		desc->release();

		return need_resize;
	}

	bool context::present()
	{
		encoder_->endEncoding();

		current_command_buffer_->presentDrawable(current_drawable_);
		current_command_buffer_->commit();

		current_command_buffer_->release();
		current_drawable_->release();

		return true;
	}

	MTL::RenderCommandEncoder* context::current_render_command()
	{
		return encoder_;
	}

	mat4 context::get_proj()
	{
		return proj_;
	}

	void context::create_depth_texture()
	{
		instance& inst = instance::get();

		MTL::TextureDescriptor* desc = MTL::TextureDescriptor::alloc()->init();
		desc->setTextureType(MTL::TextureType2D);
		desc->setPixelFormat(MTL::PixelFormatDepth32Float);

		auto [w, h] = surface_.get_size();
		desc->setWidth(w);
		desc->setHeight(h);
		desc->setUsage(MTL::TextureUsageRenderTarget);

		depth_tex_ = inst.get_device()->newTexture(desc);

		desc->release();
	}
}
