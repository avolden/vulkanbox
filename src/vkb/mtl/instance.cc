#include "instance.hh"
#include <Metal/MTLCommandQueue.hpp>
#include <Metal/MTLDevice.hpp>

namespace vkb::mtl
{
	instance* instance::instance_ {nullptr};

	instance& instance::get()
	{
		return *instance_;
	}

	instance::instance(bool enable_validation)
	{
		instance_ = this;
		device_ = MTL::CreateSystemDefaultDevice();

		queue_ = device_->newCommandQueue();

		lib_ = device_->newDefaultLibrary();

		// TODO validation
		(void)enable_validation;
	}

	instance::~instance()
	{
		queue_->release();
		device_->release();
	}

	MTL::Device* instance::get_device()
	{
		return device_;
	}

	MTL::CommandQueue* instance::get_queue()
	{
		return queue_;
	}
}