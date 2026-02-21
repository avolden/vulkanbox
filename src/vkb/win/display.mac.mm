#include "display.hh"

#include "window.hh"

#include <AppKit/AppKit.h>

namespace vkb
{
	display::display()
	{
		instance_ = this;
	}

	display::~display() {}

	void display::update()
	{
		@autoreleasepool
		{
			NSEvent* event;
			while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
											   untilDate:[NSDate distantPast]
												  inMode:NSDefaultRunLoopMode
												 dequeue:YES]))
			{
				[NSApp sendEvent:event];
			}
			[NSApp updateWindows];
		}
	}

	void display::add_window(window* win)
	{
		windows_.emplace_back(win);
	}

	void display::remove_window(window* win)
	{
		for (uint32_t i {0}; i < windows_.size(); ++i)
		{
			if (windows_[i] == win)
			{
				windows_.swap_pop_back(i);
				break;
			}
		}
	}
}