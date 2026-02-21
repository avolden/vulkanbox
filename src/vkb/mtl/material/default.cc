#include "default.hh"

#include "../../math/vec4.hh"

#include "../instance.hh"
#include <Foundation/NSError.hpp>
#include <Foundation/NSString.hpp>
#include <Metal/MTLDevice.hpp>
#include <Metal/MTLLibrary.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <Metal/MTLRenderPipeline.hpp>
#include <Metal/MTLResource.hpp>

namespace vkb::mtl
{
	triangle::triangle()
	{
		vec4 tri[3] {
			{-0.5f, -0.5f, 0.0f, 1.0f},
			{0.5f,  -0.5f, 0.0f, 1.0f},
			{0.0f,  0.5f,  0.0f, 1.0f}
        };

		instance& inst = instance::get();

		buf_ = inst.get_device()->newBuffer(tri, sizeof(tri),
		                                    MTL::ResourceStorageModeShared);

		lib_ = inst.get_device()->newLibrary(
			NS::String::string("res/shaders/metal/default.metallib",
		                       NS::ASCIIStringEncoding),
			nullptr);

		MTL::Function* vertex = lib_->newFunction(
			NS::String::string("vertexFunction", NS::ASCIIStringEncoding));
		MTL::Function* fragment = lib_->newFunction(
			NS::String::string("fragmentFunction", NS::ASCIIStringEncoding));

		MTL::RenderPipelineDescriptor* desc =
			MTL::RenderPipelineDescriptor::alloc()->init();
		desc->setLabel(NS::String::string("Triangle", NS::ASCIIStringEncoding));
		desc->setVertexFunction(vertex);
		desc->setFragmentFunction(fragment);
		// TODO either hardcode or retrieve from current surface drawn onto
		desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);

		NS::Error* err;
		pso_ = inst.get_device()->newRenderPipelineState(desc, &err);

		desc->release();
	}

	triangle::~triangle() {}

	void triangle::draw(MTL::RenderCommandEncoder* cmd)
	{
		cmd->setRenderPipelineState(pso_);
		cmd->setVertexBuffer(buf_, 0, 0);
		cmd->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::Integer(0), 3);
	}
}