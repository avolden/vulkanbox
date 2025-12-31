#define VOLK_IMPLEMENTATION
#ifdef VKB_WINDOWS
#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(VKB_LINUX)
#define VK_USE_PLATFORM_WAYLAND_KHR
#endif
#include <volk/volk.h>