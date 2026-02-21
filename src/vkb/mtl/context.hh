#pragma once

#include "../math/mat4.hh"

namespace MTL
{
	class CommandBuffer;
	class RenderCommandEncoder;
}

namespace CA
{
	class MetalDrawable;
}

namespace vkb
{
	class window;
}

namespace vkb::mtl
{
	class surface;

	class context
	{
	public:
		context(surface& surface);
		~context();

		bool created() const;

		// void set_proj(float near, float far, float fov_deg);

		// void init_model(model& model, mc::array_view<model::vert> verts,
		//                 mc::array_view<uint16_t> idcs);
		// void destroy_model(model& model);

		// void init_texture(texture& tex, mc::string_view path);
		// void destroy_texture(texture& tex);

		bool prepare_draw();
		void begin_draw();
		bool present();

		MTL::RenderCommandEncoder* current_render_command();

		// void fill_init_info(ImGui_ImplVulkan_InitInfo& init_info);

		// VkCommandBuffer current_command_buffer();
		// uint32_t        current_img_idx();

		// void wait_completion();

		mat4 get_proj();

	private:
		surface& surface_;

		CA::MetalDrawable*         current_drawable_ {nullptr};
		MTL::CommandBuffer*        current_command_buffer_ {nullptr};
		MTL::RenderCommandEncoder* encoder_ {nullptr};
	};
}