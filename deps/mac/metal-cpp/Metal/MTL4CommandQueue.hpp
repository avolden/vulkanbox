//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Metal/MTL4CommandQueue.hpp
//
// Copyright 2020-2025 Apple Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

#include "../Foundation/Foundation.hpp"
#include "MTL4CommitFeedback.hpp"
#include "MTLDefines.hpp"
#include "MTLHeaderBridge.hpp"
#include "MTLPrivate.hpp"
#include "MTLResourceStateCommandEncoder.hpp"
#include "MTLTypes.hpp"
#include <dispatch/dispatch.h>
#include <stdint.h>

namespace MTL
{
	class Buffer;
	class Device;
	class Drawable;
	class Event;
	class Heap;
	class ResidencySet;
	class Texture;
}

namespace MTL4
{
	class CommandBuffer;
	class CommandQueueDescriptor;
	class CommitOptions;
	struct CopySparseBufferMappingOperation;
	struct CopySparseTextureMappingOperation;
	struct UpdateSparseBufferMappingOperation;
	struct UpdateSparseTextureMappingOperation;
	_MTL_ENUM(NS::Integer, CommandQueueError) {
		CommandQueueErrorNone = 0,          CommandQueueErrorTimeout = 1,
		CommandQueueErrorNotPermitted = 2,  CommandQueueErrorOutOfMemory = 3,
		CommandQueueErrorDeviceRemoved = 4, CommandQueueErrorAccessRevoked = 5,
		CommandQueueErrorInternal = 6,
	};

	struct UpdateSparseTextureMappingOperation
	{
		MTL::SparseTextureMappingMode mode;
		MTL::Region                   textureRegion;
		NS::UInteger                  textureLevel;
		NS::UInteger                  textureSlice;
		NS::UInteger                  heapOffset;
	} _MTL_PACKED;

	struct CopySparseTextureMappingOperation
	{
		MTL::Region  sourceRegion;
		NS::UInteger sourceLevel;
		NS::UInteger sourceSlice;
		MTL::Origin  destinationOrigin;
		NS::UInteger destinationLevel;
		NS::UInteger destinationSlice;
	} _MTL_PACKED;

	struct UpdateSparseBufferMappingOperation
	{
		MTL::SparseTextureMappingMode mode;
		NS::Range                     bufferRange;
		NS::UInteger                  heapOffset;
	} _MTL_PACKED;

	struct CopySparseBufferMappingOperation
	{
		NS::Range    sourceRange;
		NS::UInteger destinationOffset;
	} _MTL_PACKED;

	class CommitOptions : public NS::Referencing<CommitOptions>
	{
	public:
		void addFeedbackHandler(MTL4::CommitFeedbackHandler const block);
		void addFeedbackHandler(MTL4::CommitFeedbackHandlerFunction const& function);

		static CommitOptions* alloc();

		CommitOptions* init();
	};

	class CommandQueueDescriptor : public NS::Copying<CommandQueueDescriptor>
	{
	public:
		static CommandQueueDescriptor* alloc();

		dispatch_queue_t feedbackQueue() const;

		CommandQueueDescriptor* init();

		NS::String* label() const;

		void setFeedbackQueue(dispatch_queue_t const feedbackQueue);

		void setLabel(NS::String const* label);
	};

	class CommandQueue : public NS::Referencing<CommandQueue>
	{
	public:
		void addResidencySet(MTL::ResidencySet const* residencySet);
		void addResidencySets(MTL::ResidencySet const* const residencySets[],
		                      NS::UInteger                   count);

		void commit(MTL4::CommandBuffer const* const commandBuffers[],
		            NS::UInteger                     count);
		void commit(MTL4::CommandBuffer const* const commandBuffers[], NS::UInteger count,
		            MTL4::CommitOptions const* options);

		void copyBufferMappingsFromBuffer(
			MTL::Buffer const* sourceBuffer, MTL::Buffer const* destinationBuffer,
			MTL4::CopySparseBufferMappingOperation const* operations, NS::UInteger count);

		void copyTextureMappingsFromTexture(
			MTL::Texture const* sourceTexture, MTL::Texture const* destinationTexture,
			MTL4::CopySparseTextureMappingOperation const* operations,
			NS::UInteger                                   count);

		MTL::Device* device() const;

		NS::String* label() const;

		void removeResidencySet(MTL::ResidencySet const* residencySet);
		void removeResidencySets(MTL::ResidencySet const* const residencySets[],
		                         NS::UInteger                   count);

		void signalDrawable(MTL::Drawable const* drawable);

		void signalEvent(MTL::Event const* event, uint64_t value);

		void
		updateBufferMappings(MTL::Buffer const* buffer, MTL::Heap const* heap,
		                     MTL4::UpdateSparseBufferMappingOperation const* operations,
		                     NS::UInteger                                    count);

		void
		updateTextureMappings(MTL::Texture const* texture, MTL::Heap const* heap,
		                      MTL4::UpdateSparseTextureMappingOperation const* operations,
		                      NS::UInteger                                     count);

		void wait(MTL::Event const* event, uint64_t value);
		void wait(MTL::Drawable const* drawable);
	};

}

_MTL_INLINE void
MTL4::CommitOptions::addFeedbackHandler(MTL4::CommitFeedbackHandler const block)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(addFeedbackHandler_), block);
}

_MTL_INLINE void MTL4::CommitOptions::addFeedbackHandler(
	MTL4::CommitFeedbackHandlerFunction const& function)
{
	__block MTL4::CommitFeedbackHandlerFunction blockFunction = function;
	addFeedbackHandler(^(MTL4::CommitFeedback* pFeedback) {
	  blockFunction(pFeedback);
	});
}

_MTL_INLINE MTL4::CommitOptions* MTL4::CommitOptions::alloc()
{
	return NS::Object::alloc<MTL4::CommitOptions>(_MTL_PRIVATE_CLS(MTL4CommitOptions));
}

_MTL_INLINE MTL4::CommitOptions* MTL4::CommitOptions::init()
{
	return NS::Object::init<MTL4::CommitOptions>();
}

_MTL_INLINE MTL4::CommandQueueDescriptor* MTL4::CommandQueueDescriptor::alloc()
{
	return NS::Object::alloc<MTL4::CommandQueueDescriptor>(
		_MTL_PRIVATE_CLS(MTL4CommandQueueDescriptor));
}

_MTL_INLINE dispatch_queue_t MTL4::CommandQueueDescriptor::feedbackQueue() const
{
	return Object::sendMessage<dispatch_queue_t>(this, _MTL_PRIVATE_SEL(feedbackQueue));
}

_MTL_INLINE MTL4::CommandQueueDescriptor* MTL4::CommandQueueDescriptor::init()
{
	return NS::Object::init<MTL4::CommandQueueDescriptor>();
}

_MTL_INLINE NS::String* MTL4::CommandQueueDescriptor::label() const
{
	return Object::sendMessage<NS::String*>(this, _MTL_PRIVATE_SEL(label));
}

_MTL_INLINE void
MTL4::CommandQueueDescriptor::setFeedbackQueue(dispatch_queue_t const feedbackQueue)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setFeedbackQueue_), feedbackQueue);
}

_MTL_INLINE void MTL4::CommandQueueDescriptor::setLabel(NS::String const* label)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setLabel_), label);
}

_MTL_INLINE void
MTL4::CommandQueue::addResidencySet(MTL::ResidencySet const* residencySet)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(addResidencySet_), residencySet);
}

_MTL_INLINE void MTL4::CommandQueue::addResidencySets(
	MTL::ResidencySet const* const residencySets[], NS::UInteger count)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(addResidencySets_count_),
	                          residencySets, count);
}

_MTL_INLINE void MTL4::CommandQueue::commit(
	MTL4::CommandBuffer const* const commandBuffers[], NS::UInteger count)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(commit_count_), commandBuffers,
	                          count);
}

_MTL_INLINE void
MTL4::CommandQueue::commit(MTL4::CommandBuffer const* const commandBuffers[],
                           NS::UInteger count, MTL4::CommitOptions const* options)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(commit_count_options_),
	                          commandBuffers, count, options);
}

_MTL_INLINE void MTL4::CommandQueue::copyBufferMappingsFromBuffer(
	MTL::Buffer const* sourceBuffer, MTL::Buffer const* destinationBuffer,
	MTL4::CopySparseBufferMappingOperation const* operations, NS::UInteger count)
{
	Object::sendMessage<void>(
		this, _MTL_PRIVATE_SEL(copyBufferMappingsFromBuffer_toBuffer_operations_count_),
		sourceBuffer, destinationBuffer, operations, count);
}

_MTL_INLINE void MTL4::CommandQueue::copyTextureMappingsFromTexture(
	MTL::Texture const* sourceTexture, MTL::Texture const* destinationTexture,
	MTL4::CopySparseTextureMappingOperation const* operations, NS::UInteger count)
{
	Object::sendMessage<void>(
		this,
		_MTL_PRIVATE_SEL(copyTextureMappingsFromTexture_toTexture_operations_count_),
		sourceTexture, destinationTexture, operations, count);
}

_MTL_INLINE MTL::Device* MTL4::CommandQueue::device() const
{
	return Object::sendMessage<MTL::Device*>(this, _MTL_PRIVATE_SEL(device));
}

_MTL_INLINE NS::String* MTL4::CommandQueue::label() const
{
	return Object::sendMessage<NS::String*>(this, _MTL_PRIVATE_SEL(label));
}

_MTL_INLINE void
MTL4::CommandQueue::removeResidencySet(MTL::ResidencySet const* residencySet)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(removeResidencySet_), residencySet);
}

_MTL_INLINE void MTL4::CommandQueue::removeResidencySets(
	MTL::ResidencySet const* const residencySets[], NS::UInteger count)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(removeResidencySets_count_),
	                          residencySets, count);
}

_MTL_INLINE void MTL4::CommandQueue::signalDrawable(MTL::Drawable const* drawable)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(signalDrawable_), drawable);
}

_MTL_INLINE void MTL4::CommandQueue::signalEvent(MTL::Event const* event, uint64_t value)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(signalEvent_value_), event, value);
}

_MTL_INLINE void MTL4::CommandQueue::updateBufferMappings(
	MTL::Buffer const* buffer, MTL::Heap const* heap,
	MTL4::UpdateSparseBufferMappingOperation const* operations, NS::UInteger count)
{
	Object::sendMessage<void>(
		this, _MTL_PRIVATE_SEL(updateBufferMappings_heap_operations_count_), buffer, heap,
		operations, count);
}

_MTL_INLINE void MTL4::CommandQueue::updateTextureMappings(
	MTL::Texture const* texture, MTL::Heap const* heap,
	MTL4::UpdateSparseTextureMappingOperation const* operations, NS::UInteger count)
{
	Object::sendMessage<void>(
		this, _MTL_PRIVATE_SEL(updateTextureMappings_heap_operations_count_), texture,
		heap, operations, count);
}

_MTL_INLINE void MTL4::CommandQueue::wait(MTL::Event const* event, uint64_t value)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(waitForEvent_value_), event, value);
}

_MTL_INLINE void MTL4::CommandQueue::wait(MTL::Drawable const* drawable)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(waitForDrawable_), drawable);
}
