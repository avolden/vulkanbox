#pragma once

#include "TypeTraits.hpp"
#include <stddef.h>
#include <stdint.h>

extern void* operator new(size_t count, void* ptr) noexcept;
extern void* operator new[](size_t count, void* ptr) noexcept;

namespace MtlCpp
{

	template <typename T>
	class Function;

	template <typename R, typename... Args>
	class Function<R(Args...)>
	{
	public:
		Function() = default;

		Function(Function const& other)
		{
			if (other.lambda_funcs_)
			{
				other.lambda_funcs_->copy_f((void*)other.storage_, storage_);

				lambda_funcs_ = other.lambda_funcs_;
			}
		}

		Function(Function&& other)
		{
			if (other.lambda_funcs_)
			{
				other.lambda_funcs_->move_f(other.storage_, storage_);
				other.lambda_funcs_->delete_f(other.storage_);

				lambda_funcs_ = other.lambda_funcs_;
			}
		}

		~Function()
		{
			if (lambda_funcs_)
				lambda_funcs_->delete_f(storage_);
		}

		Function& operator=(Function const& other)
		{
			if (lambda_funcs_)
				lambda_funcs_->delete_f(storage_);

			if (other.lambda_funcs_)
			{
				other.lambda_funcs_->copy_f(other.storage_, storage_);

				lambda_funcs_ = other.lambda_funcs_;
			}
		}

		Function& operator=(Function&& other)
		{
			if (lambda_funcs_)
				lambda_funcs_->delete_f(storage_);

			if (other.lambda_funcs_)
			{
				other.lambda_funcs_->move_f(other.storage_, storage_);
				other.lambda_funcs_->delete_f(other.storage_);

				lambda_funcs_ = other.lambda_funcs_;
			}

			return *this;
		}

		template <typename L,
		          enable_if_t<!__is_same(typename remove_reference<
											 typename remove_cv<L>::type>::type,
		                                 Function),
		                      int> = 0>
		Function(L&& lambda)
		{
			static_assert(sizeof(lambda) <= sizeof(void*) * 2);

			static lambda_funcs funcs {
				[](void* storage, Args&&... args)
				{ return ((L*)storage)->operator()(forward(args)...); },
				[](void* storage) { ((L*)storage)->~L(); }, [](void* from, void* to)
				{ new ((L*)to) L(static_cast<L&&>(*((L*)from))); },
				[](void* from, void* to) { new ((L*)to) L(*((L*)from)); }};

			lambda_funcs_ = &funcs;

			new ((L*)storage_) L(forward<L>(lambda));
		}

		R operator()(Args... args)
		{
			return lambda_funcs_->invoke_f(storage_, forward<Args>(args)...);
		}

	private:
		struct lambda_funcs
		{
			R (*invoke_f)(void*, Args...) = nullptr;
			void (*delete_f)(void*) = nullptr;
			void (*move_f)(void*, void*) = nullptr;
			void (*copy_f)(void*, void*) = nullptr;
		};

		lambda_funcs* lambda_funcs_ {nullptr};
		alignas(void*) uint8_t storage_[sizeof(void*) * 2];
	};
}