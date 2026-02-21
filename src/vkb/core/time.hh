#pragma once

#include <stdint.h>

#if defined(VKB_LINUX) || defined(VKB_MAC)
#include <time.h>
#endif

namespace vkb
{
	namespace time
	{
#ifdef VKB_WINDOWS
		using stamp = int64_t;
#elif defined(VKB_LINUX) || defined(VKB_MAC)
		using stamp = timespec;
#endif

		stamp now();

		double elapsed_sec(stamp start, stamp end);
		double elapsed_ms(stamp start, stamp end);
	}
}