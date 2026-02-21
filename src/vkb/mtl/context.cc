#include "context.hh"

#include "instance.hh"
#include "surface.hh"
#include <Metal/MTLCommandBuffer.hpp>
#include <Metal/MTLCommandQueue.hpp>
#include <Metal/MTLDevice.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <Metal/MTLRenderPass.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>

namespace vkb::mtl
{
	context::context(surface& surface)
	: surface_ {surface}
	{}

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
			need_resize = true;
		}
		current_drawable_ = surface_.get_drawable();

		current_command_buffer_ = inst.get_queue()->commandBuffer();
		MTL::RenderPassDescriptor* desc = MTL::RenderPassDescriptor::alloc()->init();
		MTL::RenderPassColorAttachmentDescriptor* color =
			desc->colorAttachments()->object(0);
		color->setTexture(current_drawable_->texture());
		color->setLoadAction(MTL::LoadActionClear);
		static float col = 0.1f;
		static bool  reverse = false;
		if (reverse)
		{
			col -= 0.005f;
			if (col < 0.1f)
			{
				reverse = false;
				col = 0.1f;
			}
		}
		else
		{
			col += 0.005f;
			if (col > 0.9f)
			{
				reverse = true;
				col = 0.9f;
			}
		}
		color->setClearColor(MTL::ClearColor(col, col, col, 1.0));
		color->setStoreAction(MTL::StoreActionStore);

		encoder_ = current_command_buffer_->renderCommandEncoder(desc);

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
}
