#include "surface.hh"

#include "instance.hh"

#include "../win/window.hh"

#include <AppKit/AppKit.h>

namespace vkb::mtl
{
	void surface::set_window_layer()
	{
		NSWindow* native_win = (NSWindow*)win_.native_handle();
		NSView*   view = native_win.contentView;
		view.wantsLayer = true;
		view.layer = (__bridge CALayer*)layer_;
	}
}