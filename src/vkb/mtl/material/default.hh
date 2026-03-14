#pragma once

#include <stdint.h>

namespace MTL
{
	class RenderPipelineState;
	class Library;
	class Buffer;

	class RenderCommandEncoder;
}

namespace vkb
{
	class mat4;
}

namespace vkb::mtl
{
	struct texture;
	struct model;

	class triangle
	{
	public:
		triangle();
		~triangle();

		void prepare_draw(uint32_t cur_img, mat4 const& view, mat4 const& proj);
		void draw(model const& mod, texture const& tex, uint32_t cur_img,
		          MTL::RenderCommandEncoder* cmd);

	private:
		MTL::Library*             lib_ {nullptr};
		MTL::Buffer*              model_ {nullptr};
		MTL::RenderPipelineState* pso_ {nullptr};

		MTL::Buffer* vp_[2] {nullptr};
	};
}