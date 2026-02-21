#pragma once

namespace MTL
{
	class Device;
	class Library;
	class CommandQueue;
}

namespace vkb::mtl
{
	class instance
	{
	public:
		static instance& get();

		instance(bool enable_validation);
		instance(instance const&) = delete;
		instance(instance&&) = delete;
		~instance();

		MTL::Device*       get_device();
		MTL::CommandQueue* get_queue();

	private:
		static instance* instance_;

		MTL::Device*       device_ {nullptr};
		MTL::CommandQueue* queue_ {nullptr};
		MTL::Library*      lib_ {nullptr};
	};
}