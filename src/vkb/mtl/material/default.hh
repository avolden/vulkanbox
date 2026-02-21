#pragma once

namespace MTL
{
	class RenderPipelineState;
	class Library;
	class Buffer;

	class RenderCommandEncoder;
}

namespace vkb::mtl
{
	class triangle
	{
	public:
		triangle();
		~triangle();

		void draw(MTL::RenderCommandEncoder* cmd);

	private:
		MTL::Library*             lib_ {nullptr};
		MTL::Buffer*              buf_ {nullptr};
		MTL::RenderPipelineState* pso_ {nullptr};
	};
}