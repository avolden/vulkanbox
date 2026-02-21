#include "window.hh"

namespace vkb
{
	bool window::closed() const
	{
		return closed_;
	}

	bool window::minimized() const
	{
		return min_;
	}

	void* window::native_handle() const
	{
		return handle_;
	}

	mc::pair<uint32_t, uint32_t> window::size() const
	{
		return size_;
	}

	mc::pair<uint32_t, uint32_t> window::physical_size() const
	{
		return phys_size_;
	}

	mc::pair<int32_t, int32_t> window::position() const
	{
		return pos_;
	}
}