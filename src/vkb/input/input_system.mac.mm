#include "input_system.hh"

#include "keys.hh"

#include <AppKit/AppKit.h>
#include <Carbon/Carbon.h>

namespace vkb
{
	namespace
	{
		key convert_keycode_to_key(uint16_t keycode)
		{
			switch (keycode)
			{
				case kVK_Escape: return key::esc;
				case kVK_F1: return key::f1;
				case kVK_F2: return key::f2;
				case kVK_F3: return key::f3;
				case kVK_F4: return key::f4;
				case kVK_F5: return key::f5;
				case kVK_F6: return key::f6;
				case kVK_F7: return key::f7;
				case kVK_F8: return key::f8;
				case kVK_F9: return key::f9;
				case kVK_F10: return key::f10;
				case kVK_F11: return key::f11;
				case kVK_F12: return key::f12;
				case kVK_F13: return key::prnt_scrn;
				case kVK_F14: return key::scrl_lck;
				case kVK_F15: return key::pause_brk;
				// TODO Need to handle physical layouts for this conversion. ANSI layouts
				// have different keycode for grave than ISO, since Apple moved the
				// grave on the ISO extra key in their official layouts.
				case kVK_ISO_Section: return key::grave;
				case kVK_ANSI_1: return key::one;
				case kVK_ANSI_2: return key::two;
				case kVK_ANSI_3: return key::three;
				case kVK_ANSI_4: return key::four;
				case kVK_ANSI_5: return key::five;
				case kVK_ANSI_6: return key::six;
				case kVK_ANSI_7: return key::seven;
				case kVK_ANSI_8: return key::eight;
				case kVK_ANSI_9: return key::nine;
				case kVK_ANSI_0: return key::zero;
				case kVK_ANSI_Q: return key::q;
				case kVK_ANSI_W: return key::w;
				case kVK_ANSI_E: return key::e;
				case kVK_ANSI_R: return key::r;
				case kVK_ANSI_T: return key::t;
				case kVK_ANSI_Y: return key::y;
				case kVK_ANSI_U: return key::u;
				case kVK_ANSI_I: return key::i;
				case kVK_ANSI_O: return key::o;
				case kVK_ANSI_P: return key::p;
				case kVK_ANSI_A: return key::a;
				case kVK_ANSI_S: return key::s;
				case kVK_ANSI_D: return key::d;
				case kVK_ANSI_F: return key::f;
				case kVK_ANSI_G: return key::g;
				case kVK_ANSI_H: return key::h;
				case kVK_ANSI_J: return key::j;
				case kVK_ANSI_K: return key::k;
				case kVK_ANSI_L: return key::l;
				case kVK_ANSI_Z: return key::z;
				case kVK_ANSI_X: return key::x;
				case kVK_ANSI_C: return key::c;
				case kVK_ANSI_V: return key::v;
				case kVK_ANSI_B: return key::b;
				case kVK_ANSI_N: return key::n;
				case kVK_ANSI_M: return key::m;
				case kVK_ANSI_Minus: return key::dash;
				case kVK_ANSI_Equal: return key::equal;
				case kVK_ANSI_LeftBracket: return key::l_bracket;
				case kVK_ANSI_RightBracket: return key::r_bracket;
				case kVK_ANSI_Semicolon: return key::semicolon;
				case kVK_ANSI_Quote: return key::apostrophe;
				case kVK_ANSI_Backslash: return key::backslash;
				case kVK_ANSI_Comma: return key::comma;
				case kVK_ANSI_Period: return key::period;
				case kVK_ANSI_Slash: return key::slash;
				case kVK_Tab: return key::tab;
				case kVK_CapsLock: return key::caps_lock;
				case kVK_Shift: return key::l_shift;
				case kVK_Control: return key::l_ctrl;
				case kVK_Command: return key::l_cmd;
				case kVK_Option: return key::l_opt;
				case kVK_Space: return key::space;
				case kVK_RightOption: return key::r_opt;
				case kVK_RightCommand: return key::r_cmd;
				case kVK_ContextualMenu: return key::menu;
				case kVK_RightControl: return key::r_ctrl;
				case kVK_RightShift: return key::r_shift;
				case kVK_Return: return key::enter;
				case kVK_Delete: return key::backspace;
				case kVK_Help: return key::insert;
				case kVK_Home: return key::home;
				case kVK_PageUp: return key::page_up;
				case kVK_ForwardDelete: return key::del;
				case kVK_End: return key::end;
				case kVK_PageDown: return key::page_down;
				case kVK_UpArrow: return key::up;
				case kVK_LeftArrow: return key::left;
				case kVK_DownArrow: return key::down;
				case kVK_RightArrow: return key::right;
				case kVK_ANSI_KeypadClear: return key::num_lock;
				case kVK_ANSI_KeypadDivide: return key::num_div;
				case kVK_ANSI_KeypadMultiply: return key::num_mul;
				case kVK_ANSI_KeypadMinus: return key::num_min;
				case kVK_ANSI_KeypadPlus: return key::num_add;
				case kVK_ANSI_KeypadEnter: return key::num_enter;
				case kVK_ANSI_KeypadDecimal: return key::num_period;
				case kVK_ANSI_Keypad0: return key::num_0;
				case kVK_ANSI_Keypad1: return key::num_1;
				case kVK_ANSI_Keypad2: return key::num_2;
				case kVK_ANSI_Keypad3: return key::num_3;
				case kVK_ANSI_Keypad4: return key::num_4;
				case kVK_ANSI_Keypad5: return key::num_5;
				case kVK_ANSI_Keypad6: return key::num_6;
				case kVK_ANSI_Keypad7: return key::num_7;
				case kVK_ANSI_Keypad8: return key::num_8;
				case kVK_ANSI_Keypad9: return key::num_9;
				default: return key::max_enum;
			}
		}
	} // namespace

	input_system::input_system()
	{
		memset(key_states_, 0, (mc::to_underlying(key::max_enum) * 2) / 8 + 1);
	}

	input_system::~input_system() {}

	void input_system::key_down(NSEvent* event)
	{
		key k = convert_keycode_to_key([event keyCode]);
		if (k != key::max_enum)
			set_state(k, true);
	}

	void input_system::key_up(NSEvent* event)
	{
		key k = convert_keycode_to_key([event keyCode]);
		if (k != key::max_enum)
			set_state(k, false);
	}

	void input_system::mouse_down(NSEvent* event)
	{
		key mouse_btn;
		switch ([event buttonNumber])
		{
			case 0: mouse_btn = key::m1; break;
			case 1: mouse_btn = key::m2; break;
			case 2: mouse_btn = key::m3; break;
			case 3: mouse_btn = key::m4; break;
			case 4: mouse_btn = key::m5; break;
			default: return;
		}
		set_state(mouse_btn, true);
	}

	void input_system::mouse_up(NSEvent* event)
	{
		key mouse_btn;
		switch ([event buttonNumber])
		{
			case 0: mouse_btn = key::m1; break;
			case 1: mouse_btn = key::m2; break;
			case 2: mouse_btn = key::m3; break;
			case 3: mouse_btn = key::m4; break;
			case 4: mouse_btn = key::m5; break;
			default: return;
		}
		set_state(mouse_btn, false);
	}

	void input_system::mouse_move(NSEvent* event)
	{
		// Get the delta movement from the event
		CGFloat dx = [event deltaX];
		CGFloat dy = [event deltaY];

		pos_rel_.first += static_cast<int32_t>(dx);
		pos_rel_.second += static_cast<int32_t>(dy);

		// Get absolute position in window
		NSPoint location = [event locationInWindow];
		pos_abs_.first = static_cast<int32_t>(location.x);
		pos_abs_.second = static_cast<int32_t>(location.y);
	}

	void input_system::mouse_scroll(NSEvent* event)
	{
		CGFloat scroll_x = [event scrollingDeltaX];
		CGFloat scroll_y = [event scrollingDeltaY];

		// Invert Y to match typical conventions (positive = scroll up)
		wheel_.first += static_cast<float>(scroll_x);
		wheel_.second += static_cast<float>(-scroll_y);
	}

	void input_system::flags_changed(NSEvent* event)
	{
		key k = convert_keycode_to_key(event.keyCode);
		if (k == key::l_shift || k == key::r_shift)
			set_state(k, (event.modifierFlags & NSEventModifierFlagShift) != 0);

		if (k == key::l_ctrl || k == key::r_ctrl)
			set_state(k, (event.modifierFlags & NSEventModifierFlagControl) != 0);

		if (k == key::l_alt || k == key::r_alt)
			set_state(k, (event.modifierFlags & NSEventModifierFlagOption) != 0);

		if (k == key::l_cmd || k == key::r_cmd)
			set_state(k, (event.modifierFlags & NSEventModifierFlagCommand) != 0);
	}
} // namespace vkb
