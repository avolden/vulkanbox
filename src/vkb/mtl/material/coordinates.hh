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
	struct vec2;

	namespace cam
	{
		class base;
	}
}

namespace vkb::mtl
{
	class coordinates
	{
	public:
		coordinates();
		~coordinates();

		void prepare_draw(uint32_t cur_img, cam::base const& cam, mat4 const& proj,
		                  vec2 const& translate);
		void draw(uint32_t cur_img, MTL::RenderCommandEncoder* cmd);

	private:
		MTL::Library*             lib_ {nullptr};
		MTL::RenderPipelineState* pso_ {nullptr};

		MTL::Buffer* mvp_[2] {nullptr};
	};
}