#include "context.hh"
#include "instance.hh"

#include "../cam/free.hh"
#include "../log.hh"
#include "../math/trig.hh"
#include "../win/window.hh"

#include <imgui/backends/imgui_impl_vulkan.h>
#ifdef VKB_WINDOWS
#include <vulkan/vulkan_win32.h>
#include <win32/misc.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <yyjson.h>

#ifdef VKB_WINDOWS
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <stdio.h>
#include <string.h>

namespace vkb::vk
{
	namespace
	{}

	context::context(window const& win, surface& surface)
	: win_ {win}
	, surface_ {surface}
	{
		// auto [w, h] = win_.size();
		auto [w, h] = surface_.get_extent();
		proj_ = mat4::persp_proj(near_, far_, w / (float)h, rad(fov_deg_));

		create_command_buffers();

		created_ = create_sync_objects();
		if (!created_)
		{
			log::error("Failed to create synchronization objects");
			return;
		}
	}

	context::~context()
	{
		instance& inst = instance::get();

		vkDeviceWaitIdle(inst.get_device());

		for (uint8_t i {0}; i < context::max_frames_in_flight; ++i)
		{
			if (in_flight_fences_[i])
				vkDestroyFence(inst.get_device(), in_flight_fences_[i], nullptr);
			if (draw_end_semaphores_[i])
				vkDestroySemaphore(inst.get_device(), draw_end_semaphores_[i], nullptr);
			if (img_avail_semaphores_[i])
				vkDestroySemaphore(inst.get_device(), img_avail_semaphores_[i], nullptr);
		}

		for (uint32_t i {0}; i < recycled_semaphores_.size(); ++i)
			vkDestroySemaphore(inst.get_device(), recycled_semaphores_[i], nullptr);
	}

	bool context::created() const
	{
		return created_;
	}

	void context::set_proj(float near, float far, float fov_deg)
	{
		near_ = near;
		far_ = far;
		fov_deg_ = fov_deg;

		// auto [w, h] = win_.size();
		auto [w, h] = surface_.get_extent();
		proj_ = mat4::persp_proj(near_, far_, w / (float)h, rad(fov_deg_));
	}

	void context::init_model(model& model, mc::array_view<model::vert> verts,
	                         mc::array_view<uint16_t> idcs)
	{
		instance& inst = instance::get();

		uint64_t vert_size {sizeof(model::vert) * verts.size()};
		uint64_t idcs_size {sizeof(uint16_t) * idcs.size()};

		buffer staging_vert = inst.create_buffer(
			vert_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		buffer staging_idcs = inst.create_buffer(
			idcs_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* buff_mem;
		vmaMapMemory(inst.get_allocator(), staging_vert.memory, &buff_mem);
		memcpy(buff_mem, verts.data(), vert_size);
		vmaUnmapMemory(inst.get_allocator(), staging_vert.memory);
		vmaMapMemory(inst.get_allocator(), staging_idcs.memory, &buff_mem);
		memcpy(buff_mem, idcs.data(), idcs_size);
		vmaUnmapMemory(inst.get_allocator(), staging_idcs.memory);

		model.vertex_ = inst.create_buffer(vert_size,
		                                   VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		                                       VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		                                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		model.index_ = inst.create_buffer(idcs_size,
		                                  VK_BUFFER_USAGE_TRANSFER_DST_BIT |
		                                      VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		                                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkCommandBuffer cmd = inst.begin_commands();
		inst.copy_buffer(cmd, staging_vert, model.vertex_, vert_size);
		inst.copy_buffer(cmd, staging_idcs, model.index_, idcs_size);
		inst.end_commands(cmd);

		inst.destroy_buffer(staging_vert);
		inst.destroy_buffer(staging_idcs);

		model.idcs_size_ = idcs.size();
	}

	void context::destroy_model(model& model)
	{
		instance& inst = instance::get();

		inst.destroy_buffer(model.index_);
		inst.destroy_buffer(model.vertex_);
	}

	void context::init_texture(texture& tex, mc::string_view path)
	{
		instance& inst = instance::get();

		int32_t  w, h, c;
		uint8_t* pix = stbi_load(path.data(), &w, &h, &c, STBI_rgb_alpha);
		if (!pix)
			return;

		tex.mip_lvl = floor(log2(w > h ? w : h));
		uint64_t size = w * h * 4;

		buffer staging = inst.create_buffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT |
		                                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		void* data;
		vmaMapMemory(inst.get_allocator(), staging.memory, &data);
		memcpy(data, pix, size);
		vmaUnmapMemory(inst.get_allocator(), staging.memory);

		stbi_image_free(pix);

		tex.img = inst.create_image(
			w, h, tex.mip_lvl, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
				VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkCommandBuffer cmd = inst.begin_commands();

		inst.transition_image_layout(
			cmd, tex.img.image, VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 0, VK_ACCESS_TRANSFER_WRITE_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT, tex.mip_lvl);

		VkBufferImageCopy2 copy {};
		copy.sType = VK_STRUCTURE_TYPE_BUFFER_IMAGE_COPY_2;
		copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copy.imageSubresource.layerCount = 1;
		copy.imageExtent.width = w;
		copy.imageExtent.height = h;
		copy.imageExtent.depth = 1;

		VkCopyBufferToImageInfo2 info {};
		info.sType = VK_STRUCTURE_TYPE_COPY_BUFFER_TO_IMAGE_INFO_2;
		info.srcBuffer = staging.buffer;
		info.dstImage = tex.img.image;
		info.regionCount = 1;
		info.pRegions = &copy;
		info.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		vkCmdCopyBufferToImage2(cmd, &info);
		// inst.end_commands(cmd);
		// cmd = inst.begin_commands();
		generate_mips(cmd, tex.img, VK_FORMAT_R8G8B8A8_SRGB, w, h, tex.mip_lvl);
		inst.end_commands(cmd);

		inst.destroy_buffer(staging);

		tex.img_view = inst.create_image_view(tex.img.image, VK_FORMAT_R8G8B8A8_SRGB,
		                                      VK_IMAGE_ASPECT_COLOR_BIT, tex.mip_lvl);

		VkSamplerCreateInfo sampler {};
		sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		sampler.magFilter = VK_FILTER_NEAREST;
		sampler.minFilter = VK_FILTER_NEAREST;
		sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler.minLod = 0.f;
		sampler.maxLod = tex.mip_lvl;
		sampler.mipLodBias = 0.f;

		VkPhysicalDeviceProperties props {};
		vkGetPhysicalDeviceProperties(inst.get_physical_device(), &props);
		sampler.anisotropyEnable = VK_TRUE;
		sampler.maxAnisotropy = props.limits.maxSamplerAnisotropy;

		sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
		sampler.unnormalizedCoordinates = VK_FALSE;
		sampler.compareEnable = VK_FALSE;
		sampler.compareOp = VK_COMPARE_OP_ALWAYS;
		sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		vkCreateSampler(inst.get_device(), &sampler, nullptr, &tex.sampler);
	}

	void context::destroy_texture(texture& tex)
	{
		instance& inst = instance::get();

		if (tex.sampler)
			vkDestroySampler(inst.get_device(), tex.sampler, nullptr);
		if (tex.img_view)
			vkDestroyImageView(inst.get_device(), tex.img_view, nullptr);
		if (tex.img.image && tex.img.memory)
			vmaDestroyImage(inst.get_allocator(), tex.img.image, tex.img.memory);
	}

	bool context::prepare_draw()
	{
		instance& inst = instance::get();

		VkSemaphore new_img_avail_semaphore {nullptr};
		if (recycled_semaphores_.empty())
		{
			VkSemaphoreCreateInfo info {};
			info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			vkCreateSemaphore(inst.get_device(), &info, nullptr,
			                  &new_img_avail_semaphore);
		}
		else
		{
			new_img_avail_semaphore = recycled_semaphores_.back();
			recycled_semaphores_.pop_back();
		}

		VkResult res =
			vkAcquireNextImageKHR(inst.get_device(), surface_.get_swapchain(), UINT64_MAX,
		                          new_img_avail_semaphore, VK_NULL_HANDLE, &img_idx_);

		// if (res == VK_ERROR_OUT_OF_DATE_KHR || surface_.need_swapchain_update())
		// {
		// 	recycled_semaphores_.emplace_back(new_img_avail_semaphore);
		// 	recreate_swapchain();
		// 	return false;
		// }

		vkWaitForFences(inst.get_device(), 1, &in_flight_fences_[img_idx_], VK_TRUE,
		                UINT64_MAX);
		vkResetFences(inst.get_device(), 1, &in_flight_fences_[img_idx_]);

		vkResetCommandBuffer(command_buffers_[img_idx_], 0);

		VkSemaphore old_semaphore = img_avail_semaphores_[img_idx_];
		if (old_semaphore)
			recycled_semaphores_.emplace_back(old_semaphore);
		img_avail_semaphores_[img_idx_] = new_img_avail_semaphore;

		VkCommandBufferBeginInfo begin_info {};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin_info.flags = 0;
		begin_info.pInheritanceInfo = nullptr;

		res = vkBeginCommandBuffer(command_buffers_[img_idx_], &begin_info);
		if (res != VK_SUCCESS)
			return false;

		inst.transition_image_layout(
			command_buffers_[img_idx_], surface_.get_images()[img_idx_],
			VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			0,                                            // srcAccessMask
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,         // dstAccessMask
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,            // srcStage
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT // dstStage
		);

		return true;
	}

	void context::begin_draw()
	{
		VkRenderingAttachmentInfo color_attachment {};
		color_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		color_attachment.imageView = surface_.get_image_views()[img_idx_];
		color_attachment.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		VkRenderingAttachmentInfo depth_attachment {};
		depth_attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		depth_attachment.clearValue.depthStencil = {1.f, 0};
		depth_attachment.imageView = surface_.get_depth_stencil_view();
		depth_attachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
		depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		VkRenderingInfo render_info {};
		render_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		render_info.layerCount = 1;
		render_info.colorAttachmentCount = 1;
		render_info.pColorAttachments = &color_attachment;
		render_info.pDepthAttachment = &depth_attachment;
		render_info.renderArea = {
			{0, 0},
            surface_.get_extent()
        };

		vkCmdBeginRendering(command_buffers_[img_idx_], &render_info);

		VkViewport viewport {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = surface_.get_extent().width;
		viewport.height = surface_.get_extent().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewportWithCount(command_buffers_[img_idx_], 1, &viewport);

		VkRect2D scissor {};
		scissor.offset = {0, 0};
		scissor.extent = surface_.get_extent();
		vkCmdSetScissorWithCount(command_buffers_[img_idx_], 1, &scissor);
	}

	bool context::present()
	{
		instance& inst = instance::get();

		vkCmdEndRendering(command_buffers_[img_idx_]);

		inst.transition_image_layout(
			command_buffers_[img_idx_], surface_.get_images()[img_idx_],
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,          // srcAccessMask
			0,                                             // dstAccessMask
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // srcStage
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT           // dstStage
		);

		vkEndCommandBuffer(command_buffers_[img_idx_]);
		VkSubmitInfo submit_info {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore          sem_wait[] {img_avail_semaphores_[img_idx_]};
		VkPipelineStageFlags stages_wait[] {
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = sem_wait;
		submit_info.pWaitDstStageMask = stages_wait;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffers_[img_idx_];

		VkSemaphore sem_signal[] {draw_end_semaphores_[img_idx_]};
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = sem_signal;

		vkQueueSubmit(inst.get_graphics_queue(), 1, &submit_info,
		              in_flight_fences_[img_idx_]);

		VkPresentInfoKHR present_info {};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = sem_signal;

		VkSwapchainKHR swapchains[] {surface_.get_swapchain()};
		present_info.swapchainCount = 1;
		present_info.pSwapchains = swapchains;
		present_info.pImageIndices = &img_idx_;

		bool     need_swapchain_update = false;
		VkResult res = vkQueuePresentKHR(inst.get_present_queue(), &present_info);
		need_swapchain_update = res == VK_ERROR_OUT_OF_DATE_KHR ||
		                        res == VK_SUBOPTIMAL_KHR ||
		                        surface_.need_swapchain_update();

		if (need_swapchain_update)
			recreate_swapchain();

		cur_frame_ = (cur_frame_ + 1) % context::max_frames_in_flight;

		return need_swapchain_update;
	}

	void context::fill_init_info(ImGui_ImplVulkan_InitInfo& init_info)
	{
		instance& inst = instance::get();
		init_info.Instance = inst.get_instance();
		init_info.PhysicalDevice = inst.get_physical_device();
		init_info.Device = inst.get_device();
		init_info.Queue = inst.get_graphics_queue();
		init_info.PipelineCache = nullptr;
		init_info.DescriptorPool = nullptr;
		init_info.DescriptorPoolSize = 1;
		init_info.PipelineInfoMain.RenderPass = nullptr;
		init_info.UseDynamicRendering = true;

		VkPipelineRenderingCreateInfo rendering_info {};
		rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		rendering_info.colorAttachmentCount = 1;
		surface_format_ = surface_.get_format().format;
		rendering_info.pColorAttachmentFormats = &surface_format_;
		rendering_info.depthAttachmentFormat = VK_FORMAT_D32_SFLOAT;

		init_info.PipelineInfoMain.PipelineRenderingCreateInfo = rendering_info;
		init_info.PipelineInfoMain.Subpass = 0;
		init_info.MinImageCount = context::max_frames_in_flight;
		init_info.ImageCount = context::max_frames_in_flight;
		init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.Allocator = nullptr;
	}

	VkCommandBuffer context::current_command_buffer()
	{
		return command_buffers_[img_idx_];
	}

	uint32_t context::current_img_idx()
	{
		return img_idx_;
	}

	void context::wait_completion()
	{
		instance& inst = instance::get();
		vkDeviceWaitIdle(inst.get_device());
	}

	mat4 context::get_proj()
	{
		return proj_;
	}

	void context::generate_mips(VkCommandBuffer cmd, image const& img, VkFormat format,
	                            uint32_t w, uint32_t h, uint32_t mip_lvl)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(instance::get().get_physical_device(), format,
		                                    &props);
		if (!(props.optimalTilingFeatures &
		      VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
			return;

		VkImageMemoryBarrier barrier {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = img.image;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.subresourceRange.levelCount = 1;

		int32_t mip_w = w;
		int32_t mip_h = h;
		for (uint32_t i {1}; i < mip_lvl; ++i)
		{
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
			                     VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
			                     nullptr, 1, &barrier);

			VkImageBlit blit {};
			blit.srcOffsets[0] = {0, 0, 0};
			blit.srcOffsets[1] = {mip_w, mip_h, 1};
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = 1;

			blit.dstOffsets[0] = {0, 0, 0};
			blit.dstOffsets[1] = {mip_w > 1 ? mip_w / 2 : 1, mip_h > 1 ? mip_h / 2 : 1,
			                      1};
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(cmd, img.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			               img.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit,
			               VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
			                     VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
			                     nullptr, 1, &barrier);

			if (mip_w > 1)
				mip_w /= 2;
			if (mip_h > 1)
				mip_h /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mip_lvl - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT,
		                     VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
		                     nullptr, 1, &barrier);
	}

	void context::create_command_buffers()
	{
		mc::vector<VkCommandBuffer> cmds =
			instance::get().allocate_commands(context::max_frames_in_flight);

		for (uint32_t i {0}; i < context::max_frames_in_flight; ++i)
			command_buffers_[i] = cmds[i];
	}

	bool context::create_sync_objects()
	{
		instance& inst = instance::get();

		VkSemaphoreCreateInfo sem_info {};
		sem_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fence_info {};
		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (uint8_t i {0}; i < context::max_frames_in_flight; ++i)
		{
			VkResult res1 = vkCreateSemaphore(inst.get_device(), &sem_info, nullptr,
			                                  &img_avail_semaphores_[i]);
			VkResult res2 = vkCreateSemaphore(inst.get_device(), &sem_info, nullptr,
			                                  &draw_end_semaphores_[i]);
			VkResult res3 = vkCreateFence(inst.get_device(), &fence_info, nullptr,
			                              &in_flight_fences_[i]);

			if (res1 != VK_SUCCESS && res2 != VK_SUCCESS && res3 != VK_SUCCESS)
				return false;
		}
		return true;
	}

	void context::recreate_swapchain()
	{
		vkDeviceWaitIdle(instance::get().get_device());

		surface_.destroy_swapchain();
		surface_.create_swapchain();

		auto [w, h] = surface_.get_extent();
		proj_ = mat4::persp_proj(near_, far_, w / (float)h, rad(fov_deg_));
	}
} // namespace vkb::vk