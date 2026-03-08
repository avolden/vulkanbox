#pragma once

#include <stdint.h>

#ifdef VKB_WINDOWS
#include <win32/window.h>
#endif

#include <pair.hh>
#include <string_view.hh>

namespace vkb
{
	class input_system;
	class display;
}
#ifdef VKB_LINUX
struct libdecor_frame;
struct libdecor_configuration;
#elif defined(VKB_MAC)
#ifdef __OBJC__
@class NSView;
@class NSWindow;
@class win_delegate;
class custom_view_bridge;
class win_delegate_bridge;
#else
class NSView;
class NSWindow;
class win_delegate;
class custom_view_bridge;
class win_delegate_bridge;
#endif
#endif

namespace vkb
{
	class window
	{
		friend display;
		friend custom_view_bridge;
		friend win_delegate_bridge;

	public:
		window(mc::string_view name, input_system* is = nullptr);
		window(window const&) = delete;
		window(window&&);
		~window();

		window& operator=(window const&) = delete;
		window& operator=(window&&);

		bool  closed() const;
		bool  minimized() const;
		void* native_handle() const;

		mc::pair<uint32_t, uint32_t> size() const;
		mc::pair<uint32_t, uint32_t> physical_size() const;
		mc::pair<int32_t, int32_t>   position() const;

		void lock_mouse() const;
		void unlock_mouse() const;

		void show_mouse() const;
		void hide_mouse() const;

	private:
		void update();
#ifdef VKB_WINDOWS
		static uint16_t class_id;

		static LRESULT wnd_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
#elif defined(VKB_LINUX)
		static void libdecor_configure(libdecor_frame*         frame,
		                               libdecor_configuration* configuration, void* ud);
		static void libdecor_close(libdecor_frame* frame, void* ud);
		static void libdecor_commit(libdecor_frame* frame, void* ud);
		static void libdecor_dismiss_popup(struct libdecor_frame* frame,
		                                   char const* seat_name, void* ud);

		void pointer_motion(int32_t x, int32_t y);
		void pointer_relative_motion(int32_t dx, int32_t dy, int32_t dx_raw,
		                             int32_t dy_raw);
		void pointer_button(uint32_t button, uint32_t state);
		void pointer_axis(uint32_t axis, int32_t state);

		void keyboard_key(uint32_t key, uint32_t state);

		libdecor_frame* frame_;
		uint32_t        state_;

		uint32_t w_ {0};
		uint32_t h_ {0};
#elif defined(VKB_MAC)
		NSWindow*     win_ {nullptr};
		NSView*       view_ {nullptr};
		win_delegate* delegate_ {nullptr};
#endif

		void*         handle_;
		input_system* is_;

		mc::pair<uint32_t, uint32_t> size_ {0, 0};
		mc::pair<uint32_t, uint32_t> phys_size_ {0, 0};
		mc::pair<int32_t, int32_t>   pos_ {0, 0};
		bool                         closed_ {false};
		bool                         min_ {false};
	};
}