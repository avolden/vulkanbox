#pragma once

#include "../math/mat4.hh"
#include "object.hh"

#include "material.hh"
#include "surface.hh"

#include <array_view.hh>
#include <string_view.hh>
#include <vector.hh>

#include "vma/vma.hh"
#include <volk/volk.h>

#include <stdint.h>

namespace vkb
{
	class window;

	namespace ui
	{
		class context;
	}

	namespace cam
	{
		class base;
	}
}

struct ImGui_ImplVulkan_InitInfo;

namespace vkb::vk
{
	class context
	{
		friend ui::context;

	public:
		context(window const& win, surface& surface);
		~context();

		bool created() const;

		void set_proj(float near, float far, float fov_deg);

		void init_model(model& model, mc::array_view<model::vert> verts,
		                mc::array_view<uint16_t> idcs);
		void destroy_model(model& model);

		void init_texture(texture& tex, mc::string_view path);
		void destroy_texture(texture& tex);

		bool prepare_draw();
		void begin_draw();
		bool present();

		void fill_init_info(ImGui_ImplVulkan_InitInfo& init_info);

		VkCommandBuffer current_command_buffer();
		uint32_t        current_img_idx();

		void wait_completion();

		mat4 get_proj();

	private:
		constexpr static uint8_t max_frames_in_flight {3};
		uint8_t                  cur_frame_ {0};
		uint32_t                 img_idx_ {0};

		void generate_mips(VkCommandBuffer cmd, image const& img, VkFormat format,
		                   uint32_t w, uint32_t h, uint32_t mip_lvl);

		void create_command_buffers();
		bool create_sync_objects();

		void recreate_swapchain();

		window const& win_;
		surface&      surface_;
		bool          created_ {true};

		VkFormat surface_format_;

		VkCommandBuffer command_buffers_[context::max_frames_in_flight] {nullptr};

		mc::vector<VkSemaphore> recycled_semaphores_;
		VkSemaphore img_avail_semaphores_[context::max_frames_in_flight] {nullptr};
		VkSemaphore draw_end_semaphores_[context::max_frames_in_flight] {nullptr};
		VkFence     in_flight_fences_[context::max_frames_in_flight] {nullptr};

		mat4  proj_;
		float near_ {0.1f};
		float far_ {100.f};
		float fov_deg_ {70.f};
	};
}