#include "surface.hh"

#include "../win/window.hh"
#include "instance.hh"

#include <CoreFoundation/CFCGTypes.h>
#include <CoreGraphics/CGGeometry.h>
#include <Metal/MTLPixelFormat.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

namespace vkb::mtl
{
	surface::surface(window const& win)
	: win_ {win}
	{
		instance& inst = instance::get();

		layer_ = CA::MetalLayer::layer();
		layer_->retain();
		layer_->setDevice(inst.get_device());
		layer_->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
		layer_->setFramebufferOnly(true);
		set_window_layer();

		auto [w, h] = win.physical_size();
		layer_->setDrawableSize(CGSizeMake(w, h));
	}

	surface::~surface()
	{
		layer_->release();
	}

	CA::MetalDrawable* surface::get_drawable()
	{
		return layer_->nextDrawable();
	}

	bool surface::need_resize()
	{
		CGSize drawable_size = layer_->drawableSize();
		auto [w, h] = win_.physical_size();
		return (drawable_size.width != w) || (drawable_size.height != h);
	}

	void surface::resize()
	{
		auto [w, h] = win_.physical_size();
		layer_->setDrawableSize(CGSizeMake(w, h));
	}
}