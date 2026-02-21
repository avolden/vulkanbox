#include "window.hh"
#include "pair.hh"
#include "string_view.hh"
#include "vkb/input/input_system.hh"
#include "vkb/win/display.hh"

#include <AppKit/AppKit.h>
#include <Cocoa/Cocoa.h>
#include <Foundation/Foundation.h>

@interface custom_view : NSView
{
	vkb::window* window;
}

- (instancetype)init:(NSRect)frameRect window:(vkb::window*)native_win;
@end

@implementation custom_view

- (instancetype)init:(NSRect)frameRect window:(vkb::window*)native_win
{
	self = [super initWithFrame:frameRect];
	if (self)
		window = native_win;
	return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
	[[NSColor grayColor] setStroke];
	NSRectFill(dirtyRect);
	[super drawRect:dirtyRect];
}

// TODO input events

@end

class win_delegate_bridge
{
public:
	static void window_will_close(vkb::window* win)
	{
		win->closed_ = true;
	}

	static void window_did_resize(vkb::window* win, mc::pair<uint32_t, uint32_t> size,
	                              mc::pair<uint32_t, uint32_t> phys_size)
	{
		win->size_ = size;
		win->phys_size_ = phys_size;
	}

	static void window_did_move(vkb::window* win, mc::pair<int32_t, int32_t> pos)
	{
		win->pos_ = pos;
	}
};

@interface win_delegate : NSObject <NSWindowDelegate>

{
	vkb::window* win_;
}

- (instancetype)init:(vkb::window*)native_win;
@end

@implementation win_delegate

- (instancetype)init:(vkb::window*)native_win
{
	if (self = [super init])
		win_ = native_win;
	return self;
}

- (bool)windowShouldClose:(NSWindow*)sender
{
	return true;
}

- (void)windowWillClose:(NSNotification*)notification
{
	win_delegate_bridge::window_will_close(win_);
}

- (void)windowDidResize:(NSNotification*)notification
{
	NSWindow* ns_win = notification.object;
	NSSize    size = ns_win.contentView.frame.size;
	NSSize    physical_size = [ns_win.contentView convertSizeToBacking:size];
	win_delegate_bridge::window_did_resize(
		win_, {static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height)},
		{static_cast<uint32_t>(physical_size.width),
	     static_cast<uint32_t>(physical_size.height)});
}

- (void)windowDidMove:(NSNotification*)notification
{
	NSWindow* ns_win = notification.object;
	// TODO Homogeneous coordinates for window position ?
	NSPoint pos = ns_win.frame.origin;
	win_delegate_bridge::window_did_move(
		win_, {static_cast<int32_t>(pos.x), static_cast<int32_t>(pos.y)});
}

@end

namespace vkb
{

	window::window([[maybe_unused]] mc::string_view name, input_system* is)
	: is_(is)
	{
		[NSApplication sharedApplication];

		pos_ = {100, 100};
		size_ = {800, 600};
		NSRect rect = NSMakeRect(100, 100, 800, 600);
		win_ = [[NSWindow alloc]
			initWithContentRect:rect
					  styleMask:NSWindowStyleMaskClosable | NSWindowStyleMaskTitled |
								NSWindowStyleMaskResizable
						backing:NSBackingStoreBuffered
						  defer:NO];

		auto str = [[NSString alloc] initWithBytes:name.data()
											length:name.size()
										  encoding:NSUTF8StringEncoding];
		[win_ setTitle:str];

		[win_ makeKeyAndOrderFront:nil];

		[NSApplication sharedApplication];
		[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
		[NSApp activateIgnoringOtherApps:YES];

		custom_view* view = [[custom_view alloc] init:rect window:this];
		view_ = view;
		view_.wantsLayer = YES;
		[win_ setContentView:view];
		handle_ = static_cast<void*>(win_);
		NSRect phys_rect = [view convertRectToBacking:rect];
		phys_size_ = {static_cast<uint32_t>(phys_rect.size.width),
		              static_cast<uint32_t>(phys_rect.size.height)};

		delegate_ = [[win_delegate alloc] init:this];
		[win_ setDelegate:delegate_];

		display::get().add_window(this);
	}

	window::~window()
	{
		display::get().remove_window(this);
	}

	void window::update() {}

	void window::lock_mouse() const {}

	void window::unlock_mouse() const {}

	void window::show_mouse() const {}

	void window::hide_mouse() const {}
}