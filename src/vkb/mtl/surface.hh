#pragma once

namespace CA
{
	class MetalLayer;
	class MetalDrawable;
}

namespace vkb
{
	class window;
}

namespace vkb::mtl
{
	class surface
	{
	public:
		surface(window const& win);
		~surface();

		CA::MetalDrawable* get_drawable();

		bool need_resize();
		void resize();

	private:
		void set_window_layer();

		window const&   win_;
		CA::MetalLayer* layer_ {nullptr};
	};
}