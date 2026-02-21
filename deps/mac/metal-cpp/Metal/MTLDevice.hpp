//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Metal/MTLDevice.hpp
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
#include "MTL4Counters.hpp"
#include "MTLArgument.hpp"
#include "MTLDataType.hpp"
#include "MTLDefines.hpp"
#include "MTLHeaderBridge.hpp"
#include "MTLPixelFormat.hpp"
#include "MTLPrivate.hpp"
#include "MTLResource.hpp"
#include "MTLTexture.hpp"
#include "MTLTypes.hpp"
#include <dispatch/dispatch.h>
#include <IOSurface/IOSurfaceRef.h>
#include <stdint.h>

#include <Helpers/Function.hpp>
#include <stdint.h>
#include <TargetConditionals.h>

namespace MTL
{
	class AccelerationStructure;
	class AccelerationStructureDescriptor;
	class Architecture;
	class ArgumentDescriptor;
	class ArgumentEncoder;
	class BinaryArchive;
	class BinaryArchiveDescriptor;
	class Buffer;
	class BufferBinding;
	class CommandQueue;
	class CommandQueueDescriptor;
	class CompileOptions;
	class ComputePipelineDescriptor;
	class ComputePipelineReflection;
	class ComputePipelineState;
	class CounterSampleBuffer;
	class CounterSampleBufferDescriptor;
	class DepthStencilDescriptor;
	class DepthStencilState;
	class Device;
	class DynamicLibrary;
	class Event;
	class Fence;
	class Function;
	class FunctionConstantValues;
	class FunctionHandle;
	class Heap;
	class HeapDescriptor;
	class IOCommandQueue;
	class IOCommandQueueDescriptor;
	class IOFileHandle;
	class IndirectCommandBuffer;
	class IndirectCommandBufferDescriptor;
	class Library;
	class LogState;
	class LogStateDescriptor;
	class MeshRenderPipelineDescriptor;
	class RasterizationRateMap;
	class RasterizationRateMapDescriptor;
	struct Region;
	class RenderPipelineDescriptor;
	class RenderPipelineReflection;
	class RenderPipelineState;
	class ResidencySet;
	class ResidencySetDescriptor;
	class ResourceViewPoolDescriptor;
	struct SamplePosition;
	class SamplerDescriptor;
	class SamplerState;
	class SharedEvent;
	class SharedEventHandle;
	class SharedTextureHandle;
	class StitchedLibraryDescriptor;
	class Tensor;
	class TensorDescriptor;
	class Texture;
	class TextureDescriptor;
	class TextureViewPool;
	class TileRenderPipelineDescriptor;

}

namespace MTL4
{
	class Archive;
	class ArgumentTable;
	class ArgumentTableDescriptor;
	class BinaryFunction;
	class CommandAllocator;
	class CommandAllocatorDescriptor;
	class CommandBuffer;
	class CommandQueue;
	class CommandQueueDescriptor;
	class Compiler;
	class CompilerDescriptor;
	class CounterHeap;
	class CounterHeapDescriptor;
	class PipelineDataSetSerializer;
	class PipelineDataSetSerializerDescriptor;

}

namespace MTL
{
	_MTL_ENUM(NS::Integer, IOCompressionMethod) {
		IOCompressionMethodZlib = 0,     IOCompressionMethodLZFSE = 1,
		IOCompressionMethodLZ4 = 2,      IOCompressionMethodLZMA = 3,
		IOCompressionMethodLZBitmap = 4,
	};

	_MTL_ENUM(NS::UInteger, FeatureSet) {
		FeatureSet_iOS_GPUFamily1_v1 = 0,
		FeatureSet_iOS_GPUFamily2_v1 = 1,
		FeatureSet_iOS_GPUFamily1_v2 = 2,
		FeatureSet_iOS_GPUFamily2_v2 = 3,
		FeatureSet_iOS_GPUFamily3_v1 = 4,
		FeatureSet_iOS_GPUFamily1_v3 = 5,
		FeatureSet_iOS_GPUFamily2_v3 = 6,
		FeatureSet_iOS_GPUFamily3_v2 = 7,
		FeatureSet_iOS_GPUFamily1_v4 = 8,
		FeatureSet_iOS_GPUFamily2_v4 = 9,
		FeatureSet_iOS_GPUFamily3_v3 = 10,
		FeatureSet_iOS_GPUFamily4_v1 = 11,
		FeatureSet_iOS_GPUFamily1_v5 = 12,
		FeatureSet_iOS_GPUFamily2_v5 = 13,
		FeatureSet_iOS_GPUFamily3_v4 = 14,
		FeatureSet_iOS_GPUFamily4_v2 = 15,
		FeatureSet_iOS_GPUFamily5_v1 = 16,
		FeatureSet_macOS_GPUFamily1_v1 = 10000,
		FeatureSet_OSX_GPUFamily1_v1 = 10000,
		FeatureSet_macOS_GPUFamily1_v2 = 10001,
		FeatureSet_OSX_GPUFamily1_v2 = 10001,
		FeatureSet_macOS_ReadWriteTextureTier2 = 10002,
		FeatureSet_OSX_ReadWriteTextureTier2 = 10002,
		FeatureSet_macOS_GPUFamily1_v3 = 10003,
		FeatureSet_macOS_GPUFamily1_v4 = 10004,
		FeatureSet_macOS_GPUFamily2_v1 = 10005,
		FeatureSet_watchOS_GPUFamily1_v1 = 20000,
		FeatureSet_WatchOS_GPUFamily1_v1 = 20000,
		FeatureSet_watchOS_GPUFamily2_v1 = 20001,
		FeatureSet_WatchOS_GPUFamily2_v1 = 20001,
		FeatureSet_tvOS_GPUFamily1_v1 = 30000,
		FeatureSet_TVOS_GPUFamily1_v1 = 30000,
		FeatureSet_tvOS_GPUFamily1_v2 = 30001,
		FeatureSet_tvOS_GPUFamily1_v3 = 30002,
		FeatureSet_tvOS_GPUFamily2_v1 = 30003,
		FeatureSet_tvOS_GPUFamily1_v4 = 30004,
		FeatureSet_tvOS_GPUFamily2_v2 = 30005,
	};

	_MTL_ENUM(NS::Integer, GPUFamily) {
		GPUFamilyApple1 = 1001,       GPUFamilyApple2 = 1002,
		GPUFamilyApple3 = 1003,       GPUFamilyApple4 = 1004,
		GPUFamilyApple5 = 1005,       GPUFamilyApple6 = 1006,
		GPUFamilyApple7 = 1007,       GPUFamilyApple8 = 1008,
		GPUFamilyApple9 = 1009,       GPUFamilyApple10 = 1010,
		GPUFamilyMac1 = 2001,         GPUFamilyMac2 = 2002,
		GPUFamilyCommon1 = 3001,      GPUFamilyCommon2 = 3002,
		GPUFamilyCommon3 = 3003,      GPUFamilyMacCatalyst1 = 4001,
		GPUFamilyMacCatalyst2 = 4002, GPUFamilyMetal3 = 5001,
		GPUFamilyMetal4 = 5002,
	};

	_MTL_ENUM(NS::UInteger, DeviceLocation) {
		DeviceLocationBuiltIn = 0,
		DeviceLocationSlot = 1,
		DeviceLocationExternal = 2,
		DeviceLocationUnspecified = NS::UIntegerMax,
	};

	_MTL_ENUM(NS::UInteger, ReadWriteTextureTier) {
		ReadWriteTextureTierNone = 0,
		ReadWriteTextureTier1 = 1,
		ReadWriteTextureTier2 = 2,
	};

	_MTL_ENUM(NS::UInteger, ArgumentBuffersTier) {
		ArgumentBuffersTier1 = 0,
		ArgumentBuffersTier2 = 1,
	};

	_MTL_ENUM(NS::UInteger, SparseTextureRegionAlignmentMode) {
		SparseTextureRegionAlignmentModeOutward = 0,
		SparseTextureRegionAlignmentModeInward = 1,
	};

	_MTL_ENUM(NS::UInteger, CounterSamplingPoint) {
		CounterSamplingPointAtStageBoundary = 0,
		CounterSamplingPointAtDrawBoundary = 1,
		CounterSamplingPointAtDispatchBoundary = 2,
		CounterSamplingPointAtTileDispatchBoundary = 3,
		CounterSamplingPointAtBlitBoundary = 4,
	};

	_MTL_OPTIONS(NS::UInteger, PipelineOption) {
		PipelineOptionNone = 0,
		PipelineOptionArgumentInfo = 1,
		PipelineOptionBindingInfo = 1,
		PipelineOptionBufferTypeInfo = 1 << 1,
		PipelineOptionFailOnBinaryArchiveMiss = 1 << 2,
	};

	using DeviceNotificationName = NS::String*;
	using DeviceNotificationHandlerBlock =
		void (^)(MTL::Device* pDevice, MTL::DeviceNotificationName notifyName);
	using DeviceNotificationHandlerFunction = MtlCpp::Function<void(
		MTL::Device* pDevice, MTL::DeviceNotificationName notifyName)>;
	using AutoreleasedComputePipelineReflection = MTL::ComputePipelineReflection*;
	using AutoreleasedRenderPipelineReflection = MTL::RenderPipelineReflection*;
	using NewLibraryCompletionHandler = void (^)(MTL::Library*, NS::Error*);
	using NewLibraryCompletionHandlerFunction =
		MtlCpp::Function<void(MTL::Library*, NS::Error*)>;
	using NewRenderPipelineStateCompletionHandler = void (^)(MTL::RenderPipelineState*,
	                                                         NS::Error*);
	using NewRenderPipelineStateCompletionHandlerFunction =
		MtlCpp::Function<void(MTL::RenderPipelineState*, NS::Error*)>;
	using NewRenderPipelineStateWithReflectionCompletionHandler =
		void (^)(MTL::RenderPipelineState*, MTL::RenderPipelineReflection*, NS::Error*);
	using NewRenderPipelineStateWithReflectionCompletionHandlerFunction =
		MtlCpp::Function<void(MTL::RenderPipelineState*, MTL::RenderPipelineReflection*,
	                          NS::Error*)>;
	using NewComputePipelineStateCompletionHandler = void (^)(MTL::ComputePipelineState*,
	                                                          NS::Error*);
	using NewComputePipelineStateCompletionHandlerFunction =
		MtlCpp::Function<void(MTL::ComputePipelineState*, NS::Error*)>;
	using NewComputePipelineStateWithReflectionCompletionHandler =
		void (^)(MTL::ComputePipelineState*, MTL::ComputePipelineReflection*, NS::Error*);
	using NewComputePipelineStateWithReflectionCompletionHandlerFunction =
		MtlCpp::Function<void(MTL::ComputePipelineState*, MTL::ComputePipelineReflection*,
	                          NS::Error*)>;
	using Timestamp = uint64_t;

	_MTL_CONST(DeviceNotificationName, DeviceWasAddedNotification);
	_MTL_CONST(DeviceNotificationName, DeviceRemovalRequestedNotification);
	_MTL_CONST(DeviceNotificationName, DeviceWasRemovedNotification);
	_MTL_CONST(NS::ErrorUserInfoKey, CommandBufferEncoderInfoErrorKey);
	Device*    CreateSystemDefaultDevice();
	NS::Array* CopyAllDevices();
	NS::Array* CopyAllDevicesWithObserver(NS::Object** pOutObserver,
	                                      MTL::DeviceNotificationHandlerBlock handler);
	NS::Array* CopyAllDevicesWithObserver(
		NS::Object** pOutObserver, MTL::DeviceNotificationHandlerFunction const& handler);
	void RemoveDeviceObserver(NS::Object const* pObserver);

	struct AccelerationStructureSizes
	{
		NS::UInteger accelerationStructureSize;
		NS::UInteger buildScratchBufferSize;
		NS::UInteger refitScratchBufferSize;
	} _MTL_PACKED;

	struct SizeAndAlign
	{
		NS::UInteger size;
		NS::UInteger align;
	} _MTL_PACKED;

	class ArgumentDescriptor : public NS::Copying<ArgumentDescriptor>
	{
	public:
		BindingAccess access() const;

		static ArgumentDescriptor* alloc();

		static ArgumentDescriptor* argumentDescriptor();

		NS::UInteger arrayLength() const;

		NS::UInteger constantBlockAlignment() const;

		DataType dataType() const;

		NS::UInteger index() const;

		ArgumentDescriptor* init();

		void setAccess(MTL::BindingAccess access);

		void setArrayLength(NS::UInteger arrayLength);

		void setConstantBlockAlignment(NS::UInteger constantBlockAlignment);

		void setDataType(MTL::DataType dataType);

		void setIndex(NS::UInteger index);

		void        setTextureType(MTL::TextureType textureType);
		TextureType textureType() const;
	};

	class Architecture : public NS::Copying<Architecture>
	{
	public:
		static Architecture* alloc();

		Architecture* init();

		NS::String* name() const;
	};

	class Device : public NS::Referencing<Device>
	{
	public:
		AccelerationStructureSizes accelerationStructureSizes(
			MTL::AccelerationStructureDescriptor const* descriptor);

		Architecture* architecture() const;

		bool areBarycentricCoordsSupported() const;

		bool areProgrammableSamplePositionsSupported() const;

		bool areRasterOrderGroupsSupported() const;

		ArgumentBuffersTier argumentBuffersSupport() const;

		[[deprecated("please use areBarycentricCoordsSupported instead")]]
		bool barycentricCoordsSupported() const;

		void convertSparsePixelRegions(MTL::Region const* pixelRegions,
		                               MTL::Region* tileRegions, MTL::Size tileSize,
		                               MTL::SparseTextureRegionAlignmentMode mode,
		                               NS::UInteger                          numRegions);

		void convertSparseTileRegions(MTL::Region const* tileRegions,
		                              MTL::Region* pixelRegions, MTL::Size tileSize,
		                              NS::UInteger numRegions);

		NS::Array* counterSets() const;

		NS::UInteger currentAllocatedSize() const;

		[[deprecated("please use isDepth24Stencil8PixelFormatSupported instead")]]
		bool depth24Stencil8PixelFormatSupported() const;

		FunctionHandle* functionHandle(MTL::Function const* function);
		FunctionHandle* functionHandle(MTL4::BinaryFunction const* function);

		void getDefaultSamplePositions(MTL::SamplePosition* positions,
		                               NS::UInteger         count);

		bool hasUnifiedMemory() const;

		[[deprecated("please use isHeadless instead")]]
		bool headless() const;

		SizeAndAlign heapAccelerationStructureSizeAndAlign(NS::UInteger size);
		SizeAndAlign heapAccelerationStructureSizeAndAlign(
			MTL::AccelerationStructureDescriptor const* descriptor);

		SizeAndAlign heapBufferSizeAndAlign(NS::UInteger         length,
		                                    MTL::ResourceOptions options);

		SizeAndAlign heapTextureSizeAndAlign(MTL::TextureDescriptor const* desc);

		bool isDepth24Stencil8PixelFormatSupported() const;

		bool isHeadless() const;

		bool isLowPower() const;

		bool isRemovable() const;

		DeviceLocation location() const;
		NS::UInteger   locationNumber() const;

		[[deprecated("please use isLowPower instead")]]
		bool lowPower() const;

		NS::UInteger maxArgumentBufferSamplerCount() const;

		NS::UInteger maxBufferLength() const;

		NS::UInteger maxThreadgroupMemoryLength() const;

		Size maxThreadsPerThreadgroup() const;

		uint64_t maxTransferRate() const;

		NS::UInteger maximumConcurrentCompilationTaskCount() const;

		NS::UInteger minimumLinearTextureAlignmentForPixelFormat(MTL::PixelFormat format);

		NS::UInteger minimumTextureBufferAlignmentForPixelFormat(MTL::PixelFormat format);

		NS::String* name() const;

		AccelerationStructure* newAccelerationStructure(NS::UInteger size);
		AccelerationStructure*
		newAccelerationStructure(MTL::AccelerationStructureDescriptor const* descriptor);

		MTL4::Archive* newArchive(NS::URL const* url, NS::Error** error);

		ArgumentEncoder* newArgumentEncoder(NS::Array const* arguments);
		ArgumentEncoder* newArgumentEncoder(MTL::BufferBinding const* bufferBinding);

		MTL4::ArgumentTable* newArgumentTable(
			MTL4::ArgumentTableDescriptor const* descriptor, NS::Error** error);

		BinaryArchive* newBinaryArchive(MTL::BinaryArchiveDescriptor const* descriptor,
		                                NS::Error**                         error);

		Buffer* newBuffer(NS::UInteger length, MTL::ResourceOptions options);
		Buffer* newBuffer(void const* pointer, NS::UInteger length,
		                  MTL::ResourceOptions options);
		Buffer* newBuffer(void const* pointer, NS::UInteger length,
		                  MTL::ResourceOptions options,
		                  void (^deallocator)(void*, NS::UInteger));
		Buffer* newBuffer(NS::UInteger length, MTL::ResourceOptions options,
		                  MTL::SparsePageSize placementSparsePageSize);

		MTL4::CommandAllocator* newCommandAllocator();
		MTL4::CommandAllocator* newCommandAllocator(
			MTL4::CommandAllocatorDescriptor const* descriptor, NS::Error** error);

		MTL4::CommandBuffer* newCommandBuffer();

		CommandQueue* newCommandQueue();
		CommandQueue* newCommandQueue(NS::UInteger maxCommandBufferCount);
		CommandQueue* newCommandQueue(MTL::CommandQueueDescriptor const* descriptor);

		MTL4::Compiler* newCompiler(MTL4::CompilerDescriptor const* descriptor,
		                            NS::Error**                     error);

		ComputePipelineState*
		newComputePipelineState(MTL::Function const* computeFunction, NS::Error** error);
		ComputePipelineState* newComputePipelineState(
			MTL::Function const* computeFunction, MTL::PipelineOption options,
			MTL::AutoreleasedComputePipelineReflection const* reflection,
			NS::Error**                                       error);
		void newComputePipelineState(
			MTL::Function const*                                computeFunction,
			MTL::NewComputePipelineStateCompletionHandler const completionHandler);
		void newComputePipelineState(
			MTL::Function const* computeFunction, MTL::PipelineOption options,
			MTL::NewComputePipelineStateWithReflectionCompletionHandler const
				completionHandler);
		ComputePipelineState* newComputePipelineState(
			MTL::ComputePipelineDescriptor const* descriptor, MTL::PipelineOption options,
			MTL::AutoreleasedComputePipelineReflection const* reflection,
			NS::Error**                                       error);
		void newComputePipelineState(
			MTL::ComputePipelineDescriptor const* descriptor, MTL::PipelineOption options,
			MTL::NewComputePipelineStateWithReflectionCompletionHandler const
				completionHandler);
		void newComputePipelineState(
			MTL::Function const* pFunction,
			MTL::NewComputePipelineStateCompletionHandlerFunction const&
				completionHandler);
		void newComputePipelineState(
			MTL::Function const* pFunction, MTL::PipelineOption options,
			MTL::NewComputePipelineStateWithReflectionCompletionHandlerFunction const&
				completionHandler);
		void newComputePipelineState(
			MTL::ComputePipelineDescriptor const* pDescriptor,
			MTL::PipelineOption                   options,
			MTL::NewComputePipelineStateWithReflectionCompletionHandlerFunction const&
				completionHandler);

		MTL4::CounterHeap* newCounterHeap(MTL4::CounterHeapDescriptor const* descriptor,
		                                  NS::Error**                        error);

		CounterSampleBuffer* newCounterSampleBuffer(
			MTL::CounterSampleBufferDescriptor const* descriptor, NS::Error** error);

		Library* newDefaultLibrary();
		Library* newDefaultLibrary(NS::Bundle const* bundle, NS::Error** error);

		DepthStencilState*
		newDepthStencilState(MTL::DepthStencilDescriptor const* descriptor);

		DynamicLibrary* newDynamicLibrary(MTL::Library const* library, NS::Error** error);
		DynamicLibrary* newDynamicLibrary(NS::URL const* url, NS::Error** error);

		Event* newEvent();

		Fence* newFence();

		Heap* newHeap(MTL::HeapDescriptor const* descriptor);

		IOCommandQueue* newIOCommandQueue(MTL::IOCommandQueueDescriptor const* descriptor,
		                                  NS::Error**                          error);

		IOFileHandle* newIOFileHandle(NS::URL const* url, NS::Error** error);
		IOFileHandle* newIOFileHandle(NS::URL const*           url,
		                              MTL::IOCompressionMethod compressionMethod,
		                              NS::Error**              error);

		IOFileHandle* newIOHandle(NS::URL const* url, NS::Error** error);
		IOFileHandle* newIOHandle(NS::URL const*           url,
		                          MTL::IOCompressionMethod compressionMethod,
		                          NS::Error**              error);

		IndirectCommandBuffer*
		newIndirectCommandBuffer(MTL::IndirectCommandBufferDescriptor const* descriptor,
		                         NS::UInteger maxCount, MTL::ResourceOptions options);

		Library* newLibrary(NS::String const* filepath, NS::Error** error);
		Library* newLibrary(NS::URL const* url, NS::Error** error);
		Library* newLibrary(dispatch_data_t const data, NS::Error** error);
		Library* newLibrary(NS::String const* source, MTL::CompileOptions const* options,
		                    NS::Error** error);
		void     newLibrary(NS::String const* source, MTL::CompileOptions const* options,
		                    MTL::NewLibraryCompletionHandler const completionHandler);
		Library* newLibrary(MTL::StitchedLibraryDescriptor const* descriptor,
		                    NS::Error**                           error);
		void     newLibrary(MTL::StitchedLibraryDescriptor const*  descriptor,
		                    MTL::NewLibraryCompletionHandler const completionHandler);
		void
		newLibrary(NS::String const* pSource, MTL::CompileOptions const* pOptions,
		           MTL::NewLibraryCompletionHandlerFunction const& completionHandler);
		void
		newLibrary(MTL::StitchedLibraryDescriptor const*           pDescriptor,
		           MTL::NewLibraryCompletionHandlerFunction const& completionHandler);

		LogState* newLogState(MTL::LogStateDescriptor const* descriptor,
		                      NS::Error**                    error);

		MTL4::CommandQueue* newMTL4CommandQueue();
		MTL4::CommandQueue* newMTL4CommandQueue(
			MTL4::CommandQueueDescriptor const* descriptor, NS::Error** error);

		MTL4::PipelineDataSetSerializer* newPipelineDataSetSerializer(
			MTL4::PipelineDataSetSerializerDescriptor const* descriptor);

		RasterizationRateMap*
		newRasterizationRateMap(MTL::RasterizationRateMapDescriptor const* descriptor);

		RenderPipelineState* newRenderPipelineState(
			MTL::RenderPipelineDescriptor const* descriptor, NS::Error** error);
		RenderPipelineState* newRenderPipelineState(
			MTL::RenderPipelineDescriptor const* descriptor, MTL::PipelineOption options,
			MTL::AutoreleasedRenderPipelineReflection const* reflection,
			NS::Error**                                      error);
		void newRenderPipelineState(
			MTL::RenderPipelineDescriptor const*               descriptor,
			MTL::NewRenderPipelineStateCompletionHandler const completionHandler);
		void newRenderPipelineState(
			MTL::RenderPipelineDescriptor const* descriptor, MTL::PipelineOption options,
			MTL::NewRenderPipelineStateWithReflectionCompletionHandler const
				completionHandler);
		RenderPipelineState* newRenderPipelineState(
			MTL::TileRenderPipelineDescriptor const*         descriptor,
			MTL::PipelineOption                              options,
			MTL::AutoreleasedRenderPipelineReflection const* reflection,
			NS::Error**                                      error);
		void newRenderPipelineState(
			MTL::TileRenderPipelineDescriptor const* descriptor,
			MTL::PipelineOption                      options,
			MTL::NewRenderPipelineStateWithReflectionCompletionHandler const
				completionHandler);
		RenderPipelineState* newRenderPipelineState(
			MTL::MeshRenderPipelineDescriptor const*         descriptor,
			MTL::PipelineOption                              options,
			MTL::AutoreleasedRenderPipelineReflection const* reflection,
			NS::Error**                                      error);
		void newRenderPipelineState(
			MTL::MeshRenderPipelineDescriptor const* descriptor,
			MTL::PipelineOption                      options,
			MTL::NewRenderPipelineStateWithReflectionCompletionHandler const
				completionHandler);
		void
			 newRenderPipelineState(MTL::RenderPipelineDescriptor const* pDescriptor,
		                            MTL::NewRenderPipelineStateCompletionHandlerFunction const&
		                                completionHandler);
		void newRenderPipelineState(
			MTL::RenderPipelineDescriptor const* pDescriptor, MTL::PipelineOption options,
			MTL::NewRenderPipelineStateWithReflectionCompletionHandlerFunction const&
				completionHandler);
		void newRenderPipelineState(
			MTL::TileRenderPipelineDescriptor const* pDescriptor,
			MTL::PipelineOption                      options,
			MTL::NewRenderPipelineStateWithReflectionCompletionHandlerFunction const&
				completionHandler);

		ResidencySet* newResidencySet(MTL::ResidencySetDescriptor const* desc,
		                              NS::Error**                        error);

		SamplerState* newSamplerState(MTL::SamplerDescriptor const* descriptor);

		SharedEvent* newSharedEvent();
		SharedEvent* newSharedEvent(MTL::SharedEventHandle const* sharedEventHandle);

		Texture* newSharedTexture(MTL::TextureDescriptor const* descriptor);
		Texture* newSharedTexture(MTL::SharedTextureHandle const* sharedHandle);

		Tensor* newTensor(MTL::TensorDescriptor const* descriptor, NS::Error** error);

		Texture*         newTexture(MTL::TextureDescriptor const* descriptor);
		Texture*         newTexture(MTL::TextureDescriptor const* descriptor,
		                            IOSurfaceRef const iosurface, NS::UInteger plane);
		TextureViewPool* newTextureViewPool(
			MTL::ResourceViewPoolDescriptor const* descriptor, NS::Error** error);

		uint32_t peerCount() const;

		uint64_t peerGroupID() const;

		uint32_t peerIndex() const;

		[[deprecated("please use areProgrammableSamplePositionsSupported instead")]]
		bool programmableSamplePositionsSupported() const;

		uint64_t queryTimestampFrequency();

		[[deprecated("please use areRasterOrderGroupsSupported instead")]]
		bool rasterOrderGroupsSupported() const;

		ReadWriteTextureTier readWriteTextureSupport() const;

		uint64_t recommendedMaxWorkingSetSize() const;

		uint64_t registryID() const;

		[[deprecated("please use isRemovable instead")]]
		bool removable() const;

		void sampleTimestamps(MTL::Timestamp* cpuTimestamp, MTL::Timestamp* gpuTimestamp);

		void
		setShouldMaximizeConcurrentCompilation(bool shouldMaximizeConcurrentCompilation);
		bool shouldMaximizeConcurrentCompilation() const;

		NS::UInteger sizeOfCounterHeapEntry(MTL4::CounterHeapType type);

		Size sparseTileSize(MTL::TextureType textureType, MTL::PixelFormat pixelFormat,
		                    NS::UInteger sampleCount);
		Size sparseTileSize(MTL::TextureType textureType, MTL::PixelFormat pixelFormat,
		                    NS::UInteger sampleCount, MTL::SparsePageSize sparsePageSize);
		NS::UInteger sparseTileSizeInBytes() const;
		NS::UInteger sparseTileSizeInBytes(MTL::SparsePageSize sparsePageSize);

		bool supports32BitFloatFiltering() const;

		bool supports32BitMSAA() const;

		bool supportsBCTextureCompression() const;

		bool supportsCounterSampling(MTL::CounterSamplingPoint samplingPoint);

		bool supportsDynamicLibraries() const;

		bool supportsFamily(MTL::GPUFamily gpuFamily);

		bool supportsFeatureSet(MTL::FeatureSet featureSet);

		bool supportsFunctionPointers() const;
		bool supportsFunctionPointersFromRender() const;

		bool supportsPrimitiveMotionBlur() const;

		bool supportsPullModelInterpolation() const;

		bool supportsQueryTextureLOD() const;

		bool supportsRasterizationRateMap(NS::UInteger layerCount);

		bool supportsRaytracing() const;
		bool supportsRaytracingFromRender() const;

		bool supportsRenderDynamicLibraries() const;

		bool supportsShaderBarycentricCoordinates() const;

		bool supportsTextureSampleCount(NS::UInteger sampleCount);

		bool supportsVertexAmplificationCount(NS::UInteger count);

		SizeAndAlign tensorSizeAndAlign(MTL::TensorDescriptor const* descriptor);
	};

} // namespace MTL

#if defined(MTL_PRIVATE_IMPLEMENTATION)
extern "C" MTL::Device* MTLCreateSystemDefaultDevice();
extern "C" NS::Array*   MTLCopyAllDevices();
extern "C" NS::Array*   MTLCopyAllDevicesWithObserver(NS::Object**,
                                                      MTL::DeviceNotificationHandlerBlock);
extern "C" void         MTLRemoveDeviceObserver(NS::Object const*);
_MTL_PRIVATE_DEF_WEAK_CONST(MTL::DeviceNotificationName, DeviceWasAddedNotification);
_MTL_PRIVATE_DEF_WEAK_CONST(MTL::DeviceNotificationName,
                            DeviceRemovalRequestedNotification);
_MTL_PRIVATE_DEF_WEAK_CONST(MTL::DeviceNotificationName, DeviceWasRemovedNotification);
_MTL_PRIVATE_DEF_CONST(NS::ErrorUserInfoKey, CommandBufferEncoderInfoErrorKey);

_NS_EXPORT MTL::Device* MTL::CreateSystemDefaultDevice()
{
	return ::MTLCreateSystemDefaultDevice();
}

_NS_EXPORT NS::Array* MTL::CopyAllDevices()
{
#if (__IPHONE_OS_VERSION_MIN_REQUIRED >= 180000) ||                                      \
	(__MAC_OS_X_VERSION_MIN_REQUIRED >= 101100)
	return ::MTLCopyAllDevices();
#else
	return nullptr;
#endif
}

_NS_EXPORT NS::Array* MTL::CopyAllDevicesWithObserver(
	NS::Object** pOutObserver, MTL::DeviceNotificationHandlerBlock handler)
{
#if TARGET_OS_OSX
	return ::MTLCopyAllDevicesWithObserver(pOutObserver, handler);
#else
	(void)pOutObserver;
	(void)handler;
	return nullptr;
#endif // TARGET_OS_OSX
}

_NS_EXPORT NS::Array* MTL::CopyAllDevicesWithObserver(
	NS::Object** pOutObserver, MTL::DeviceNotificationHandlerFunction const& handler)
{
	__block DeviceNotificationHandlerFunction function = handler;
	return CopyAllDevicesWithObserver(
		pOutObserver, ^(Device* pDevice, DeviceNotificationName pNotificationName) {
		  function(pDevice, pNotificationName);
		});
}

_NS_EXPORT void MTL::RemoveDeviceObserver(NS::Object const* pObserver)
{
	(void)pObserver;
#if TARGET_OS_OSX
	::MTLRemoveDeviceObserver(pObserver);
#endif // TARGET_OS_OSX
}

#endif // MTL_PRIVATE_IMPLEMENTATION

_MTL_INLINE MTL::BindingAccess MTL::ArgumentDescriptor::access() const
{
	return Object::sendMessage<MTL::BindingAccess>(this, _MTL_PRIVATE_SEL(access));
}

_MTL_INLINE MTL::ArgumentDescriptor* MTL::ArgumentDescriptor::alloc()
{
	return NS::Object::alloc<MTL::ArgumentDescriptor>(
		_MTL_PRIVATE_CLS(MTLArgumentDescriptor));
}

_MTL_INLINE MTL::ArgumentDescriptor* MTL::ArgumentDescriptor::argumentDescriptor()
{
	return Object::sendMessage<MTL::ArgumentDescriptor*>(
		_MTL_PRIVATE_CLS(MTLArgumentDescriptor), _MTL_PRIVATE_SEL(argumentDescriptor));
}

_MTL_INLINE NS::UInteger MTL::ArgumentDescriptor::arrayLength() const
{
	return Object::sendMessage<NS::UInteger>(this, _MTL_PRIVATE_SEL(arrayLength));
}

_MTL_INLINE NS::UInteger MTL::ArgumentDescriptor::constantBlockAlignment() const
{
	return Object::sendMessage<NS::UInteger>(this,
	                                         _MTL_PRIVATE_SEL(constantBlockAlignment));
}

_MTL_INLINE MTL::DataType MTL::ArgumentDescriptor::dataType() const
{
	return Object::sendMessage<MTL::DataType>(this, _MTL_PRIVATE_SEL(dataType));
}

_MTL_INLINE NS::UInteger MTL::ArgumentDescriptor::index() const
{
	return Object::sendMessage<NS::UInteger>(this, _MTL_PRIVATE_SEL(index));
}

_MTL_INLINE MTL::ArgumentDescriptor* MTL::ArgumentDescriptor::init()
{
	return NS::Object::init<MTL::ArgumentDescriptor>();
}

_MTL_INLINE void MTL::ArgumentDescriptor::setAccess(MTL::BindingAccess access)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setAccess_), access);
}

_MTL_INLINE void MTL::ArgumentDescriptor::setArrayLength(NS::UInteger arrayLength)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setArrayLength_), arrayLength);
}

_MTL_INLINE void
MTL::ArgumentDescriptor::setConstantBlockAlignment(NS::UInteger constantBlockAlignment)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setConstantBlockAlignment_),
	                          constantBlockAlignment);
}

_MTL_INLINE void MTL::ArgumentDescriptor::setDataType(MTL::DataType dataType)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setDataType_), dataType);
}

_MTL_INLINE void MTL::ArgumentDescriptor::setIndex(NS::UInteger index)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setIndex_), index);
}

_MTL_INLINE void MTL::ArgumentDescriptor::setTextureType(MTL::TextureType textureType)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setTextureType_), textureType);
}

_MTL_INLINE MTL::TextureType MTL::ArgumentDescriptor::textureType() const
{
	return Object::sendMessage<MTL::TextureType>(this, _MTL_PRIVATE_SEL(textureType));
}

_MTL_INLINE MTL::Architecture* MTL::Architecture::alloc()
{
	return NS::Object::alloc<MTL::Architecture>(_MTL_PRIVATE_CLS(MTLArchitecture));
}

_MTL_INLINE MTL::Architecture* MTL::Architecture::init()
{
	return NS::Object::init<MTL::Architecture>();
}

_MTL_INLINE NS::String* MTL::Architecture::name() const
{
	return Object::sendMessage<NS::String*>(this, _MTL_PRIVATE_SEL(name));
}

_MTL_INLINE MTL::AccelerationStructureSizes MTL::Device::accelerationStructureSizes(
	MTL::AccelerationStructureDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::AccelerationStructureSizes>(
		this, _MTL_PRIVATE_SEL(accelerationStructureSizesWithDescriptor_), descriptor);
}

_MTL_INLINE MTL::Architecture* MTL::Device::architecture() const
{
	return Object::sendMessage<MTL::Architecture*>(this, _MTL_PRIVATE_SEL(architecture));
}

_MTL_INLINE bool MTL::Device::areBarycentricCoordsSupported() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(areBarycentricCoordsSupported));
}

_MTL_INLINE bool MTL::Device::areProgrammableSamplePositionsSupported() const
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(areProgrammableSamplePositionsSupported));
}

_MTL_INLINE bool MTL::Device::areRasterOrderGroupsSupported() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(areRasterOrderGroupsSupported));
}

_MTL_INLINE MTL::ArgumentBuffersTier MTL::Device::argumentBuffersSupport() const
{
	return Object::sendMessage<MTL::ArgumentBuffersTier>(
		this, _MTL_PRIVATE_SEL(argumentBuffersSupport));
}

_MTL_INLINE bool MTL::Device::barycentricCoordsSupported() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(areBarycentricCoordsSupported));
}

_MTL_INLINE void MTL::Device::convertSparsePixelRegions(
	MTL::Region const* pixelRegions, MTL::Region* tileRegions, MTL::Size tileSize,
	MTL::SparseTextureRegionAlignmentMode mode, NS::UInteger numRegions)
{
	Object::sendMessage<void>(
		this,
		_MTL_PRIVATE_SEL(
			convertSparsePixelRegions_toTileRegions_withTileSize_alignmentMode_numRegions_),
		pixelRegions, tileRegions, tileSize, mode, numRegions);
}

_MTL_INLINE void MTL::Device::convertSparseTileRegions(MTL::Region const* tileRegions,
                                                       MTL::Region*       pixelRegions,
                                                       MTL::Size          tileSize,
                                                       NS::UInteger       numRegions)
{
	Object::sendMessage<void>(
		this,
		_MTL_PRIVATE_SEL(
			convertSparseTileRegions_toPixelRegions_withTileSize_numRegions_),
		tileRegions, pixelRegions, tileSize, numRegions);
}

_MTL_INLINE NS::Array* MTL::Device::counterSets() const
{
	return Object::sendMessage<NS::Array*>(this, _MTL_PRIVATE_SEL(counterSets));
}

_MTL_INLINE NS::UInteger MTL::Device::currentAllocatedSize() const
{
	return Object::sendMessage<NS::UInteger>(this,
	                                         _MTL_PRIVATE_SEL(currentAllocatedSize));
}

_MTL_INLINE bool MTL::Device::depth24Stencil8PixelFormatSupported() const
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(isDepth24Stencil8PixelFormatSupported));
}

_MTL_INLINE MTL::FunctionHandle*
			MTL::Device::functionHandle(MTL::Function const* function)
{
	return Object::sendMessage<MTL::FunctionHandle*>(
		this, _MTL_PRIVATE_SEL(functionHandleWithFunction_), function);
}

_MTL_INLINE MTL::FunctionHandle*
			MTL::Device::functionHandle(MTL4::BinaryFunction const* function)
{
	return Object::sendMessage<MTL::FunctionHandle*>(
		this, _MTL_PRIVATE_SEL(functionHandleWithBinaryFunction_), function);
}

_MTL_INLINE void MTL::Device::getDefaultSamplePositions(MTL::SamplePosition* positions,
                                                        NS::UInteger         count)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(getDefaultSamplePositions_count_),
	                          positions, count);
}

_MTL_INLINE bool MTL::Device::hasUnifiedMemory() const
{
	return Object::sendMessage<bool>(this, _MTL_PRIVATE_SEL(hasUnifiedMemory));
}

_MTL_INLINE bool MTL::Device::headless() const
{
	return Object::sendMessage<bool>(this, _MTL_PRIVATE_SEL(isHeadless));
}

_MTL_INLINE MTL::SizeAndAlign
			MTL::Device::heapAccelerationStructureSizeAndAlign(NS::UInteger size)
{
	return Object::sendMessage<MTL::SizeAndAlign>(
		this, _MTL_PRIVATE_SEL(heapAccelerationStructureSizeAndAlignWithSize_), size);
}

_MTL_INLINE MTL::SizeAndAlign MTL::Device::heapAccelerationStructureSizeAndAlign(
	MTL::AccelerationStructureDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::SizeAndAlign>(
		this, _MTL_PRIVATE_SEL(heapAccelerationStructureSizeAndAlignWithDescriptor_),
		descriptor);
}

_MTL_INLINE MTL::SizeAndAlign
MTL::Device::heapBufferSizeAndAlign(NS::UInteger length, MTL::ResourceOptions options)
{
	return Object::sendMessage<MTL::SizeAndAlign>(
		this, _MTL_PRIVATE_SEL(heapBufferSizeAndAlignWithLength_options_), length,
		options);
}

_MTL_INLINE MTL::SizeAndAlign
			MTL::Device::heapTextureSizeAndAlign(MTL::TextureDescriptor const* desc)
{
	return Object::sendMessage<MTL::SizeAndAlign>(
		this, _MTL_PRIVATE_SEL(heapTextureSizeAndAlignWithDescriptor_), desc);
}

_MTL_INLINE bool MTL::Device::isDepth24Stencil8PixelFormatSupported() const
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(isDepth24Stencil8PixelFormatSupported));
}

_MTL_INLINE bool MTL::Device::isHeadless() const
{
	return Object::sendMessage<bool>(this, _MTL_PRIVATE_SEL(isHeadless));
}

_MTL_INLINE bool MTL::Device::isLowPower() const
{
	return Object::sendMessage<bool>(this, _MTL_PRIVATE_SEL(isLowPower));
}

_MTL_INLINE bool MTL::Device::isRemovable() const
{
	return Object::sendMessage<bool>(this, _MTL_PRIVATE_SEL(isRemovable));
}

_MTL_INLINE MTL::DeviceLocation MTL::Device::location() const
{
	return Object::sendMessage<MTL::DeviceLocation>(this, _MTL_PRIVATE_SEL(location));
}

_MTL_INLINE NS::UInteger MTL::Device::locationNumber() const
{
	return Object::sendMessage<NS::UInteger>(this, _MTL_PRIVATE_SEL(locationNumber));
}

_MTL_INLINE bool MTL::Device::lowPower() const
{
	return Object::sendMessage<bool>(this, _MTL_PRIVATE_SEL(isLowPower));
}

_MTL_INLINE NS::UInteger MTL::Device::maxArgumentBufferSamplerCount() const
{
	return Object::sendMessage<NS::UInteger>(
		this, _MTL_PRIVATE_SEL(maxArgumentBufferSamplerCount));
}

_MTL_INLINE NS::UInteger MTL::Device::maxBufferLength() const
{
	return Object::sendMessage<NS::UInteger>(this, _MTL_PRIVATE_SEL(maxBufferLength));
}

_MTL_INLINE NS::UInteger MTL::Device::maxThreadgroupMemoryLength() const
{
	return Object::sendMessage<NS::UInteger>(
		this, _MTL_PRIVATE_SEL(maxThreadgroupMemoryLength));
}

_MTL_INLINE MTL::Size MTL::Device::maxThreadsPerThreadgroup() const
{
	return Object::sendMessage<MTL::Size>(this,
	                                      _MTL_PRIVATE_SEL(maxThreadsPerThreadgroup));
}

_MTL_INLINE uint64_t MTL::Device::maxTransferRate() const
{
	return Object::sendMessage<uint64_t>(this, _MTL_PRIVATE_SEL(maxTransferRate));
}

_MTL_INLINE NS::UInteger MTL::Device::maximumConcurrentCompilationTaskCount() const
{
	return Object::sendMessage<NS::UInteger>(
		this, _MTL_PRIVATE_SEL(maximumConcurrentCompilationTaskCount));
}

_MTL_INLINE NS::UInteger
MTL::Device::minimumLinearTextureAlignmentForPixelFormat(MTL::PixelFormat format)
{
	return Object::sendMessage<NS::UInteger>(
		this, _MTL_PRIVATE_SEL(minimumLinearTextureAlignmentForPixelFormat_), format);
}

_MTL_INLINE NS::UInteger
MTL::Device::minimumTextureBufferAlignmentForPixelFormat(MTL::PixelFormat format)
{
	return Object::sendMessage<NS::UInteger>(
		this, _MTL_PRIVATE_SEL(minimumTextureBufferAlignmentForPixelFormat_), format);
}

_MTL_INLINE NS::String* MTL::Device::name() const
{
	return Object::sendMessage<NS::String*>(this, _MTL_PRIVATE_SEL(name));
}

_MTL_INLINE MTL::AccelerationStructure*
			MTL::Device::newAccelerationStructure(NS::UInteger size)
{
	return Object::sendMessage<MTL::AccelerationStructure*>(
		this, _MTL_PRIVATE_SEL(newAccelerationStructureWithSize_), size);
}

_MTL_INLINE MTL::AccelerationStructure* MTL::Device::newAccelerationStructure(
	MTL::AccelerationStructureDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::AccelerationStructure*>(
		this, _MTL_PRIVATE_SEL(newAccelerationStructureWithDescriptor_), descriptor);
}

_MTL_INLINE MTL4::Archive* MTL::Device::newArchive(NS::URL const* url, NS::Error** error)
{
	return Object::sendMessage<MTL4::Archive*>(
		this, _MTL_PRIVATE_SEL(newArchiveWithURL_error_), url, error);
}

_MTL_INLINE MTL::ArgumentEncoder*
			MTL::Device::newArgumentEncoder(NS::Array const* arguments)
{
	return Object::sendMessage<MTL::ArgumentEncoder*>(
		this, _MTL_PRIVATE_SEL(newArgumentEncoderWithArguments_), arguments);
}

_MTL_INLINE MTL::ArgumentEncoder*
			MTL::Device::newArgumentEncoder(MTL::BufferBinding const* bufferBinding)
{
	return Object::sendMessage<MTL::ArgumentEncoder*>(
		this, _MTL_PRIVATE_SEL(newArgumentEncoderWithBufferBinding_), bufferBinding);
}

_MTL_INLINE MTL4::ArgumentTable* MTL::Device::newArgumentTable(
	MTL4::ArgumentTableDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL4::ArgumentTable*>(
		this, _MTL_PRIVATE_SEL(newArgumentTableWithDescriptor_error_), descriptor, error);
}

_MTL_INLINE MTL::BinaryArchive* MTL::Device::newBinaryArchive(
	MTL::BinaryArchiveDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL::BinaryArchive*>(
		this, _MTL_PRIVATE_SEL(newBinaryArchiveWithDescriptor_error_), descriptor, error);
}

_MTL_INLINE MTL::Buffer* MTL::Device::newBuffer(NS::UInteger         length,
                                                MTL::ResourceOptions options)
{
	return Object::sendMessage<MTL::Buffer*>(
		this, _MTL_PRIVATE_SEL(newBufferWithLength_options_), length, options);
}

_MTL_INLINE MTL::Buffer* MTL::Device::newBuffer(void const* pointer, NS::UInteger length,
                                                MTL::ResourceOptions options)
{
	return Object::sendMessage<MTL::Buffer*>(
		this, _MTL_PRIVATE_SEL(newBufferWithBytes_length_options_), pointer, length,
		options);
}

_MTL_INLINE MTL::Buffer* MTL::Device::newBuffer(void const* pointer, NS::UInteger length,
                                                MTL::ResourceOptions options,
                                                void (^deallocator)(void*, NS::UInteger))
{
	return Object::sendMessage<MTL::Buffer*>(
		this, _MTL_PRIVATE_SEL(newBufferWithBytesNoCopy_length_options_deallocator_),
		pointer, length, options, deallocator);
}

_MTL_INLINE MTL::Buffer*
			MTL::Device::newBuffer(NS::UInteger length, MTL::ResourceOptions options,
                                   MTL::SparsePageSize placementSparsePageSize)
{
	return Object::sendMessage<MTL::Buffer*>(
		this, _MTL_PRIVATE_SEL(newBufferWithLength_options_placementSparsePageSize_),
		length, options, placementSparsePageSize);
}

_MTL_INLINE MTL4::CommandAllocator* MTL::Device::newCommandAllocator()
{
	return Object::sendMessage<MTL4::CommandAllocator*>(
		this, _MTL_PRIVATE_SEL(newCommandAllocator));
}

_MTL_INLINE MTL4::CommandAllocator* MTL::Device::newCommandAllocator(
	MTL4::CommandAllocatorDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL4::CommandAllocator*>(
		this, _MTL_PRIVATE_SEL(newCommandAllocatorWithDescriptor_error_), descriptor,
		error);
}

_MTL_INLINE MTL4::CommandBuffer* MTL::Device::newCommandBuffer()
{
	return Object::sendMessage<MTL4::CommandBuffer*>(this,
	                                                 _MTL_PRIVATE_SEL(newCommandBuffer));
}

_MTL_INLINE MTL::CommandQueue* MTL::Device::newCommandQueue()
{
	return Object::sendMessage<MTL::CommandQueue*>(this,
	                                               _MTL_PRIVATE_SEL(newCommandQueue));
}

_MTL_INLINE MTL::CommandQueue*
			MTL::Device::newCommandQueue(NS::UInteger maxCommandBufferCount)
{
	return Object::sendMessage<MTL::CommandQueue*>(
		this, _MTL_PRIVATE_SEL(newCommandQueueWithMaxCommandBufferCount_),
		maxCommandBufferCount);
}

_MTL_INLINE MTL::CommandQueue*
			MTL::Device::newCommandQueue(MTL::CommandQueueDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::CommandQueue*>(
		this, _MTL_PRIVATE_SEL(newCommandQueueWithDescriptor_), descriptor);
}

_MTL_INLINE MTL4::Compiler*
MTL::Device::newCompiler(MTL4::CompilerDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL4::Compiler*>(
		this, _MTL_PRIVATE_SEL(newCompilerWithDescriptor_error_), descriptor, error);
}

_MTL_INLINE MTL::ComputePipelineState* MTL::Device::newComputePipelineState(
	MTL::Function const* computeFunction, NS::Error** error)
{
	return Object::sendMessage<MTL::ComputePipelineState*>(
		this, _MTL_PRIVATE_SEL(newComputePipelineStateWithFunction_error_),
		computeFunction, error);
}

_MTL_INLINE MTL::ComputePipelineState* MTL::Device::newComputePipelineState(
	MTL::Function const* computeFunction, MTL::PipelineOption options,
	MTL::AutoreleasedComputePipelineReflection const* reflection, NS::Error** error)
{
	return Object::sendMessage<MTL::ComputePipelineState*>(
		this,
		_MTL_PRIVATE_SEL(newComputePipelineStateWithFunction_options_reflection_error_),
		computeFunction, options, reflection, error);
}

_MTL_INLINE void MTL::Device::newComputePipelineState(
	MTL::Function const*                                computeFunction,
	MTL::NewComputePipelineStateCompletionHandler const completionHandler)
{
	Object::sendMessage<void>(
		this, _MTL_PRIVATE_SEL(newComputePipelineStateWithFunction_completionHandler_),
		computeFunction, completionHandler);
}

_MTL_INLINE void MTL::Device::newComputePipelineState(
	MTL::Function const* computeFunction, MTL::PipelineOption options,
	MTL::NewComputePipelineStateWithReflectionCompletionHandler const completionHandler)
{
	Object::sendMessage<void>(
		this,
		_MTL_PRIVATE_SEL(newComputePipelineStateWithFunction_options_completionHandler_),
		computeFunction, options, completionHandler);
}

_MTL_INLINE MTL::ComputePipelineState* MTL::Device::newComputePipelineState(
	MTL::ComputePipelineDescriptor const* descriptor, MTL::PipelineOption options,
	MTL::AutoreleasedComputePipelineReflection const* reflection, NS::Error** error)
{
	return Object::sendMessage<MTL::ComputePipelineState*>(
		this,
		_MTL_PRIVATE_SEL(newComputePipelineStateWithDescriptor_options_reflection_error_),
		descriptor, options, reflection, error);
}

_MTL_INLINE void MTL::Device::newComputePipelineState(
	MTL::ComputePipelineDescriptor const* descriptor, MTL::PipelineOption options,
	MTL::NewComputePipelineStateWithReflectionCompletionHandler const completionHandler)
{
	Object::sendMessage<void>(
		this,
		_MTL_PRIVATE_SEL(
			newComputePipelineStateWithDescriptor_options_completionHandler_),
		descriptor, options, completionHandler);
}

_MTL_INLINE void MTL::Device::newComputePipelineState(
	MTL::Function const*                                         pFunction,
	MTL::NewComputePipelineStateCompletionHandlerFunction const& completionHandler)
{
	__block MTL::NewComputePipelineStateCompletionHandlerFunction blockCompletionHandler =
		completionHandler;
	newComputePipelineState(
		pFunction, ^(MTL::ComputePipelineState* pPipelineState, NS::Error* pError) {
		  blockCompletionHandler(pPipelineState, pError);
		});
}

_MTL_INLINE void MTL::Device::newComputePipelineState(
	MTL::Function const* pFunction, MTL::PipelineOption options,
	MTL::NewComputePipelineStateWithReflectionCompletionHandlerFunction const&
		completionHandler)
{
	__block MTL::NewComputePipelineStateWithReflectionCompletionHandlerFunction
			blockCompletionHandler = completionHandler;
	newComputePipelineState(pFunction, options,
	                        ^(MTL::ComputePipelineState*      pPipelineState,
	                          MTL::ComputePipelineReflection* pReflection,
	                          NS::Error*                      pError) {
							  blockCompletionHandler(pPipelineState, pReflection, pError);
							});
}

_MTL_INLINE void MTL::Device::newComputePipelineState(
	MTL::ComputePipelineDescriptor const* pDescriptor, MTL::PipelineOption options,
	MTL::NewComputePipelineStateWithReflectionCompletionHandlerFunction const&
		completionHandler)
{
	__block NewComputePipelineStateWithReflectionCompletionHandlerFunction
		blockCompletionHandler = completionHandler;
	newComputePipelineState(pDescriptor, options,
	                        ^(ComputePipelineState*      pPipelineState,
	                          ComputePipelineReflection* pReflection, NS::Error* pError) {
							  blockCompletionHandler(pPipelineState, pReflection, pError);
							});
}

_MTL_INLINE MTL4::CounterHeap* MTL::Device::newCounterHeap(
	MTL4::CounterHeapDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL4::CounterHeap*>(
		this, _MTL_PRIVATE_SEL(newCounterHeapWithDescriptor_error_), descriptor, error);
}

_MTL_INLINE MTL::CounterSampleBuffer* MTL::Device::newCounterSampleBuffer(
	MTL::CounterSampleBufferDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL::CounterSampleBuffer*>(
		this, _MTL_PRIVATE_SEL(newCounterSampleBufferWithDescriptor_error_), descriptor,
		error);
}

_MTL_INLINE MTL::Library* MTL::Device::newDefaultLibrary()
{
	return Object::sendMessage<MTL::Library*>(this, _MTL_PRIVATE_SEL(newDefaultLibrary));
}

_MTL_INLINE MTL::Library* MTL::Device::newDefaultLibrary(NS::Bundle const* bundle,
                                                         NS::Error**       error)
{
	return Object::sendMessage<MTL::Library*>(
		this, _MTL_PRIVATE_SEL(newDefaultLibraryWithBundle_error_), bundle, error);
}

_MTL_INLINE MTL::DepthStencilState*
MTL::Device::newDepthStencilState(MTL::DepthStencilDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::DepthStencilState*>(
		this, _MTL_PRIVATE_SEL(newDepthStencilStateWithDescriptor_), descriptor);
}

_MTL_INLINE MTL::DynamicLibrary*
			MTL::Device::newDynamicLibrary(MTL::Library const* library, NS::Error** error)
{
	return Object::sendMessage<MTL::DynamicLibrary*>(
		this, _MTL_PRIVATE_SEL(newDynamicLibrary_error_), library, error);
}

_MTL_INLINE MTL::DynamicLibrary* MTL::Device::newDynamicLibrary(NS::URL const* url,
                                                                NS::Error**    error)
{
	return Object::sendMessage<MTL::DynamicLibrary*>(
		this, _MTL_PRIVATE_SEL(newDynamicLibraryWithURL_error_), url, error);
}

_MTL_INLINE MTL::Event* MTL::Device::newEvent()
{
	return Object::sendMessage<MTL::Event*>(this, _MTL_PRIVATE_SEL(newEvent));
}

_MTL_INLINE MTL::Fence* MTL::Device::newFence()
{
	return Object::sendMessage<MTL::Fence*>(this, _MTL_PRIVATE_SEL(newFence));
}

_MTL_INLINE MTL::Heap* MTL::Device::newHeap(MTL::HeapDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::Heap*>(this, _MTL_PRIVATE_SEL(newHeapWithDescriptor_),
	                                       descriptor);
}

_MTL_INLINE MTL::IOCommandQueue* MTL::Device::newIOCommandQueue(
	MTL::IOCommandQueueDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL::IOCommandQueue*>(
		this, _MTL_PRIVATE_SEL(newIOCommandQueueWithDescriptor_error_), descriptor,
		error);
}

_MTL_INLINE MTL::IOFileHandle* MTL::Device::newIOFileHandle(NS::URL const* url,
                                                            NS::Error**    error)
{
	return Object::sendMessage<MTL::IOFileHandle*>(
		this, _MTL_PRIVATE_SEL(newIOFileHandleWithURL_error_), url, error);
}

_MTL_INLINE MTL::IOFileHandle* MTL::Device::newIOFileHandle(
	NS::URL const* url, MTL::IOCompressionMethod compressionMethod, NS::Error** error)
{
	return Object::sendMessage<MTL::IOFileHandle*>(
		this, _MTL_PRIVATE_SEL(newIOFileHandleWithURL_compressionMethod_error_), url,
		compressionMethod, error);
}

_MTL_INLINE MTL::IOFileHandle* MTL::Device::newIOHandle(NS::URL const* url,
                                                        NS::Error**    error)
{
	return Object::sendMessage<MTL::IOFileHandle*>(
		this, _MTL_PRIVATE_SEL(newIOHandleWithURL_error_), url, error);
}

_MTL_INLINE MTL::IOFileHandle* MTL::Device::newIOHandle(
	NS::URL const* url, MTL::IOCompressionMethod compressionMethod, NS::Error** error)
{
	return Object::sendMessage<MTL::IOFileHandle*>(
		this, _MTL_PRIVATE_SEL(newIOHandleWithURL_compressionMethod_error_), url,
		compressionMethod, error);
}

_MTL_INLINE MTL::IndirectCommandBuffer* MTL::Device::newIndirectCommandBuffer(
	MTL::IndirectCommandBufferDescriptor const* descriptor, NS::UInteger maxCount,
	MTL::ResourceOptions options)
{
	return Object::sendMessage<MTL::IndirectCommandBuffer*>(
		this,
		_MTL_PRIVATE_SEL(newIndirectCommandBufferWithDescriptor_maxCommandCount_options_),
		descriptor, maxCount, options);
}

_MTL_INLINE MTL::Library* MTL::Device::newLibrary(NS::String const* filepath,
                                                  NS::Error**       error)
{
	return Object::sendMessage<MTL::Library*>(
		this, _MTL_PRIVATE_SEL(newLibraryWithFile_error_), filepath, error);
}

_MTL_INLINE MTL::Library* MTL::Device::newLibrary(NS::URL const* url, NS::Error** error)
{
	return Object::sendMessage<MTL::Library*>(
		this, _MTL_PRIVATE_SEL(newLibraryWithURL_error_), url, error);
}

_MTL_INLINE MTL::Library* MTL::Device::newLibrary(dispatch_data_t const data,
                                                  NS::Error**           error)
{
	return Object::sendMessage<MTL::Library*>(
		this, _MTL_PRIVATE_SEL(newLibraryWithData_error_), data, error);
}

_MTL_INLINE MTL::Library* MTL::Device::newLibrary(NS::String const*          source,
                                                  MTL::CompileOptions const* options,
                                                  NS::Error**                error)
{
	return Object::sendMessage<MTL::Library*>(
		this, _MTL_PRIVATE_SEL(newLibraryWithSource_options_error_), source, options,
		error);
}

_MTL_INLINE void
MTL::Device::newLibrary(NS::String const* source, MTL::CompileOptions const* options,
                        MTL::NewLibraryCompletionHandler const completionHandler)
{
	Object::sendMessage<void>(
		this, _MTL_PRIVATE_SEL(newLibraryWithSource_options_completionHandler_), source,
		options, completionHandler);
}

_MTL_INLINE MTL::Library* MTL::Device::newLibrary(
	MTL::StitchedLibraryDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL::Library*>(
		this, _MTL_PRIVATE_SEL(newLibraryWithStitchedDescriptor_error_), descriptor,
		error);
}

_MTL_INLINE void
MTL::Device::newLibrary(MTL::StitchedLibraryDescriptor const*  descriptor,
                        MTL::NewLibraryCompletionHandler const completionHandler)
{
	Object::sendMessage<void>(
		this, _MTL_PRIVATE_SEL(newLibraryWithStitchedDescriptor_completionHandler_),
		descriptor, completionHandler);
}

_MTL_INLINE void
MTL::Device::newLibrary(NS::String const* pSource, MTL::CompileOptions const* pOptions,
                        MTL::NewLibraryCompletionHandlerFunction const& completionHandler)
{
	__block MTL::NewLibraryCompletionHandlerFunction blockCompletionHandler =
		completionHandler;
	newLibrary(pSource, pOptions, ^(MTL::Library* pLibrary, NS::Error* pError) {
	  blockCompletionHandler(pLibrary, pError);
	});
}

_MTL_INLINE void
MTL::Device::newLibrary(MTL::StitchedLibraryDescriptor const*           pDescriptor,
                        MTL::NewLibraryCompletionHandlerFunction const& completionHandler)
{
	__block MTL::NewLibraryCompletionHandlerFunction blockCompletionHandler =
		completionHandler;
	newLibrary(pDescriptor, ^(MTL::Library* pLibrary, NS::Error* pError) {
	  blockCompletionHandler(pLibrary, pError);
	});
}

_MTL_INLINE MTL::LogState*
MTL::Device::newLogState(MTL::LogStateDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL::LogState*>(
		this, _MTL_PRIVATE_SEL(newLogStateWithDescriptor_error_), descriptor, error);
}

_MTL_INLINE MTL4::CommandQueue* MTL::Device::newMTL4CommandQueue()
{
	return Object::sendMessage<MTL4::CommandQueue*>(
		this, _MTL_PRIVATE_SEL(newMTL4CommandQueue));
}

_MTL_INLINE MTL4::CommandQueue* MTL::Device::newMTL4CommandQueue(
	MTL4::CommandQueueDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL4::CommandQueue*>(
		this, _MTL_PRIVATE_SEL(newMTL4CommandQueueWithDescriptor_error_), descriptor,
		error);
}

_MTL_INLINE MTL4::PipelineDataSetSerializer* MTL::Device::newPipelineDataSetSerializer(
	MTL4::PipelineDataSetSerializerDescriptor const* descriptor)
{
	return Object::sendMessage<MTL4::PipelineDataSetSerializer*>(
		this, _MTL_PRIVATE_SEL(newPipelineDataSetSerializerWithDescriptor_), descriptor);
}

_MTL_INLINE MTL::RasterizationRateMap* MTL::Device::newRasterizationRateMap(
	MTL::RasterizationRateMapDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::RasterizationRateMap*>(
		this, _MTL_PRIVATE_SEL(newRasterizationRateMapWithDescriptor_), descriptor);
}

_MTL_INLINE MTL::RenderPipelineState* MTL::Device::newRenderPipelineState(
	MTL::RenderPipelineDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL::RenderPipelineState*>(
		this, _MTL_PRIVATE_SEL(newRenderPipelineStateWithDescriptor_error_), descriptor,
		error);
}

_MTL_INLINE MTL::RenderPipelineState* MTL::Device::newRenderPipelineState(
	MTL::RenderPipelineDescriptor const* descriptor, MTL::PipelineOption options,
	MTL::AutoreleasedRenderPipelineReflection const* reflection, NS::Error** error)
{
	return Object::sendMessage<MTL::RenderPipelineState*>(
		this,
		_MTL_PRIVATE_SEL(newRenderPipelineStateWithDescriptor_options_reflection_error_),
		descriptor, options, reflection, error);
}

_MTL_INLINE void MTL::Device::newRenderPipelineState(
	MTL::RenderPipelineDescriptor const*               descriptor,
	MTL::NewRenderPipelineStateCompletionHandler const completionHandler)
{
	Object::sendMessage<void>(
		this, _MTL_PRIVATE_SEL(newRenderPipelineStateWithDescriptor_completionHandler_),
		descriptor, completionHandler);
}

_MTL_INLINE void MTL::Device::newRenderPipelineState(
	MTL::RenderPipelineDescriptor const* descriptor, MTL::PipelineOption options,
	MTL::NewRenderPipelineStateWithReflectionCompletionHandler const completionHandler)
{
	Object::sendMessage<void>(
		this,
		_MTL_PRIVATE_SEL(newRenderPipelineStateWithDescriptor_options_completionHandler_),
		descriptor, options, completionHandler);
}

_MTL_INLINE MTL::RenderPipelineState* MTL::Device::newRenderPipelineState(
	MTL::TileRenderPipelineDescriptor const* descriptor, MTL::PipelineOption options,
	MTL::AutoreleasedRenderPipelineReflection const* reflection, NS::Error** error)
{
	return Object::sendMessage<MTL::RenderPipelineState*>(
		this,
		_MTL_PRIVATE_SEL(
			newRenderPipelineStateWithTileDescriptor_options_reflection_error_),
		descriptor, options, reflection, error);
}

_MTL_INLINE void MTL::Device::newRenderPipelineState(
	MTL::TileRenderPipelineDescriptor const* descriptor, MTL::PipelineOption options,
	MTL::NewRenderPipelineStateWithReflectionCompletionHandler const completionHandler)
{
	Object::sendMessage<void>(
		this,
		_MTL_PRIVATE_SEL(
			newRenderPipelineStateWithTileDescriptor_options_completionHandler_),
		descriptor, options, completionHandler);
}

_MTL_INLINE MTL::RenderPipelineState* MTL::Device::newRenderPipelineState(
	MTL::MeshRenderPipelineDescriptor const* descriptor, MTL::PipelineOption options,
	MTL::AutoreleasedRenderPipelineReflection const* reflection, NS::Error** error)
{
	return Object::sendMessage<MTL::RenderPipelineState*>(
		this,
		_MTL_PRIVATE_SEL(
			newRenderPipelineStateWithMeshDescriptor_options_reflection_error_),
		descriptor, options, reflection, error);
}

_MTL_INLINE void MTL::Device::newRenderPipelineState(
	MTL::MeshRenderPipelineDescriptor const* descriptor, MTL::PipelineOption options,
	MTL::NewRenderPipelineStateWithReflectionCompletionHandler const completionHandler)
{
	Object::sendMessage<void>(
		this,
		_MTL_PRIVATE_SEL(
			newRenderPipelineStateWithMeshDescriptor_options_completionHandler_),
		descriptor, options, completionHandler);
}

_MTL_INLINE void MTL::Device::newRenderPipelineState(
	MTL::RenderPipelineDescriptor const*                        pDescriptor,
	MTL::NewRenderPipelineStateCompletionHandlerFunction const& completionHandler)
{
	__block MTL::NewRenderPipelineStateCompletionHandlerFunction blockCompletionHandler =
		completionHandler;
	newRenderPipelineState(
		pDescriptor, ^(MTL::RenderPipelineState* pPipelineState, NS::Error* pError) {
		  blockCompletionHandler(pPipelineState, pError);
		});
}

_MTL_INLINE void MTL::Device::newRenderPipelineState(
	MTL::RenderPipelineDescriptor const* pDescriptor, MTL::PipelineOption options,
	MTL::NewRenderPipelineStateWithReflectionCompletionHandlerFunction const&
		completionHandler)
{
	__block MTL::NewRenderPipelineStateWithReflectionCompletionHandlerFunction
			blockCompletionHandler = completionHandler;
	newRenderPipelineState(pDescriptor, options,
	                       ^(MTL::RenderPipelineState*      pPipelineState,
	                         MTL::RenderPipelineReflection* pReflection,
	                         NS::Error*                     pError) {
							 blockCompletionHandler(pPipelineState, pReflection, pError);
						   });
}

_MTL_INLINE void MTL::Device::newRenderPipelineState(
	MTL::TileRenderPipelineDescriptor const* pDescriptor, MTL::PipelineOption options,
	MTL::NewRenderPipelineStateWithReflectionCompletionHandlerFunction const&
		completionHandler)
{
	__block MTL::NewRenderPipelineStateWithReflectionCompletionHandlerFunction
			blockCompletionHandler = completionHandler;
	newRenderPipelineState(pDescriptor, options,
	                       ^(MTL::RenderPipelineState*      pPipelineState,
	                         MTL::RenderPipelineReflection* pReflection,
	                         NS::Error*                     pError) {
							 blockCompletionHandler(pPipelineState, pReflection, pError);
						   });
}

_MTL_INLINE MTL::ResidencySet*
MTL::Device::newResidencySet(MTL::ResidencySetDescriptor const* desc, NS::Error** error)
{
	return Object::sendMessage<MTL::ResidencySet*>(
		this, _MTL_PRIVATE_SEL(newResidencySetWithDescriptor_error_), desc, error);
}

_MTL_INLINE MTL::SamplerState*
			MTL::Device::newSamplerState(MTL::SamplerDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::SamplerState*>(
		this, _MTL_PRIVATE_SEL(newSamplerStateWithDescriptor_), descriptor);
}

_MTL_INLINE MTL::SharedEvent* MTL::Device::newSharedEvent()
{
	return Object::sendMessage<MTL::SharedEvent*>(this, _MTL_PRIVATE_SEL(newSharedEvent));
}

_MTL_INLINE MTL::SharedEvent*
			MTL::Device::newSharedEvent(MTL::SharedEventHandle const* sharedEventHandle)
{
	return Object::sendMessage<MTL::SharedEvent*>(
		this, _MTL_PRIVATE_SEL(newSharedEventWithHandle_), sharedEventHandle);
}

_MTL_INLINE MTL::Texture*
			MTL::Device::newSharedTexture(MTL::TextureDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::Texture*>(
		this, _MTL_PRIVATE_SEL(newSharedTextureWithDescriptor_), descriptor);
}

_MTL_INLINE MTL::Texture*
			MTL::Device::newSharedTexture(MTL::SharedTextureHandle const* sharedHandle)
{
	return Object::sendMessage<MTL::Texture*>(
		this, _MTL_PRIVATE_SEL(newSharedTextureWithHandle_), sharedHandle);
}

_MTL_INLINE MTL::Tensor* MTL::Device::newTensor(MTL::TensorDescriptor const* descriptor,
                                                NS::Error**                  error)
{
	return Object::sendMessage<MTL::Tensor*>(
		this, _MTL_PRIVATE_SEL(newTensorWithDescriptor_error_), descriptor, error);
}

_MTL_INLINE MTL::Texture*
			MTL::Device::newTexture(MTL::TextureDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::Texture*>(
		this, _MTL_PRIVATE_SEL(newTextureWithDescriptor_), descriptor);
}

_MTL_INLINE MTL::Texture*
			MTL::Device::newTexture(MTL::TextureDescriptor const* descriptor,
                                    IOSurfaceRef const iosurface, NS::UInteger plane)
{
	return Object::sendMessage<MTL::Texture*>(
		this, _MTL_PRIVATE_SEL(newTextureWithDescriptor_iosurface_plane_), descriptor,
		iosurface, plane);
}

_MTL_INLINE MTL::TextureViewPool* MTL::Device::newTextureViewPool(
	MTL::ResourceViewPoolDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL::TextureViewPool*>(
		this, _MTL_PRIVATE_SEL(newTextureViewPoolWithDescriptor_error_), descriptor,
		error);
}

_MTL_INLINE uint32_t MTL::Device::peerCount() const
{
	return Object::sendMessage<uint32_t>(this, _MTL_PRIVATE_SEL(peerCount));
}

_MTL_INLINE uint64_t MTL::Device::peerGroupID() const
{
	return Object::sendMessage<uint64_t>(this, _MTL_PRIVATE_SEL(peerGroupID));
}

_MTL_INLINE uint32_t MTL::Device::peerIndex() const
{
	return Object::sendMessage<uint32_t>(this, _MTL_PRIVATE_SEL(peerIndex));
}

_MTL_INLINE bool MTL::Device::programmableSamplePositionsSupported() const
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(areProgrammableSamplePositionsSupported));
}

_MTL_INLINE uint64_t MTL::Device::queryTimestampFrequency()
{
	return Object::sendMessage<uint64_t>(this, _MTL_PRIVATE_SEL(queryTimestampFrequency));
}

_MTL_INLINE bool MTL::Device::rasterOrderGroupsSupported() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(areRasterOrderGroupsSupported));
}

_MTL_INLINE MTL::ReadWriteTextureTier MTL::Device::readWriteTextureSupport() const
{
	return Object::sendMessage<MTL::ReadWriteTextureTier>(
		this, _MTL_PRIVATE_SEL(readWriteTextureSupport));
}

_MTL_INLINE uint64_t MTL::Device::recommendedMaxWorkingSetSize() const
{
	return Object::sendMessage<uint64_t>(this,
	                                     _MTL_PRIVATE_SEL(recommendedMaxWorkingSetSize));
}

_MTL_INLINE uint64_t MTL::Device::registryID() const
{
	return Object::sendMessage<uint64_t>(this, _MTL_PRIVATE_SEL(registryID));
}

_MTL_INLINE bool MTL::Device::removable() const
{
	return Object::sendMessage<bool>(this, _MTL_PRIVATE_SEL(isRemovable));
}

_MTL_INLINE void MTL::Device::sampleTimestamps(MTL::Timestamp* cpuTimestamp,
                                               MTL::Timestamp* gpuTimestamp)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(sampleTimestamps_gpuTimestamp_),
	                          cpuTimestamp, gpuTimestamp);
}

_MTL_INLINE void MTL::Device::setShouldMaximizeConcurrentCompilation(
	bool shouldMaximizeConcurrentCompilation)
{
	Object::sendMessage<void>(this,
	                          _MTL_PRIVATE_SEL(setShouldMaximizeConcurrentCompilation_),
	                          shouldMaximizeConcurrentCompilation);
}

_MTL_INLINE bool MTL::Device::shouldMaximizeConcurrentCompilation() const
{
	return Object::sendMessage<bool>(
		this, _MTL_PRIVATE_SEL(shouldMaximizeConcurrentCompilation));
}

_MTL_INLINE NS::UInteger MTL::Device::sizeOfCounterHeapEntry(MTL4::CounterHeapType type)
{
	return Object::sendMessage<NS::UInteger>(
		this, _MTL_PRIVATE_SEL(sizeOfCounterHeapEntry_), type);
}

_MTL_INLINE MTL::Size MTL::Device::sparseTileSize(MTL::TextureType textureType,
                                                  MTL::PixelFormat pixelFormat,
                                                  NS::UInteger     sampleCount)
{
	return Object::sendMessage<MTL::Size>(
		this, _MTL_PRIVATE_SEL(sparseTileSizeWithTextureType_pixelFormat_sampleCount_),
		textureType, pixelFormat, sampleCount);
}

_MTL_INLINE MTL::Size MTL::Device::sparseTileSize(MTL::TextureType    textureType,
                                                  MTL::PixelFormat    pixelFormat,
                                                  NS::UInteger        sampleCount,
                                                  MTL::SparsePageSize sparsePageSize)
{
	return Object::sendMessage<MTL::Size>(
		this,
		_MTL_PRIVATE_SEL(
			sparseTileSizeWithTextureType_pixelFormat_sampleCount_sparsePageSize_),
		textureType, pixelFormat, sampleCount, sparsePageSize);
}

_MTL_INLINE NS::UInteger MTL::Device::sparseTileSizeInBytes() const
{
	return Object::sendMessage<NS::UInteger>(this,
	                                         _MTL_PRIVATE_SEL(sparseTileSizeInBytes));
}

_MTL_INLINE NS::UInteger
			MTL::Device::sparseTileSizeInBytes(MTL::SparsePageSize sparsePageSize)
{
	return Object::sendMessage<NS::UInteger>(
		this, _MTL_PRIVATE_SEL(sparseTileSizeInBytesForSparsePageSize_), sparsePageSize);
}

_MTL_INLINE bool MTL::Device::supports32BitFloatFiltering() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(supports32BitFloatFiltering));
}

_MTL_INLINE bool MTL::Device::supports32BitMSAA() const
{
	return Object::sendMessageSafe<bool>(this, _MTL_PRIVATE_SEL(supports32BitMSAA));
}

_MTL_INLINE bool MTL::Device::supportsBCTextureCompression() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(supportsBCTextureCompression));
}

_MTL_INLINE bool
MTL::Device::supportsCounterSampling(MTL::CounterSamplingPoint samplingPoint)
{
	return Object::sendMessageSafe<bool>(this, _MTL_PRIVATE_SEL(supportsCounterSampling_),
	                                     samplingPoint);
}

_MTL_INLINE bool MTL::Device::supportsDynamicLibraries() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(supportsDynamicLibraries));
}

_MTL_INLINE bool MTL::Device::supportsFamily(MTL::GPUFamily gpuFamily)
{
	return Object::sendMessageSafe<bool>(this, _MTL_PRIVATE_SEL(supportsFamily_),
	                                     gpuFamily);
}

_MTL_INLINE bool MTL::Device::supportsFeatureSet(MTL::FeatureSet featureSet)
{
	return Object::sendMessageSafe<bool>(this, _MTL_PRIVATE_SEL(supportsFeatureSet_),
	                                     featureSet);
}

_MTL_INLINE bool MTL::Device::supportsFunctionPointers() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(supportsFunctionPointers));
}

_MTL_INLINE bool MTL::Device::supportsFunctionPointersFromRender() const
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(supportsFunctionPointersFromRender));
}

_MTL_INLINE bool MTL::Device::supportsPrimitiveMotionBlur() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(supportsPrimitiveMotionBlur));
}

_MTL_INLINE bool MTL::Device::supportsPullModelInterpolation() const
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(supportsPullModelInterpolation));
}

_MTL_INLINE bool MTL::Device::supportsQueryTextureLOD() const
{
	return Object::sendMessageSafe<bool>(this, _MTL_PRIVATE_SEL(supportsQueryTextureLOD));
}

_MTL_INLINE bool MTL::Device::supportsRasterizationRateMap(NS::UInteger layerCount)
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(supportsRasterizationRateMapWithLayerCount_), layerCount);
}

_MTL_INLINE bool MTL::Device::supportsRaytracing() const
{
	return Object::sendMessageSafe<bool>(this, _MTL_PRIVATE_SEL(supportsRaytracing));
}

_MTL_INLINE bool MTL::Device::supportsRaytracingFromRender() const
{
	return Object::sendMessageSafe<bool>(this,
	                                     _MTL_PRIVATE_SEL(supportsRaytracingFromRender));
}

_MTL_INLINE bool MTL::Device::supportsRenderDynamicLibraries() const
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(supportsRenderDynamicLibraries));
}

_MTL_INLINE bool MTL::Device::supportsShaderBarycentricCoordinates() const
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(supportsShaderBarycentricCoordinates));
}

_MTL_INLINE bool MTL::Device::supportsTextureSampleCount(NS::UInteger sampleCount)
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(supportsTextureSampleCount_), sampleCount);
}

_MTL_INLINE bool MTL::Device::supportsVertexAmplificationCount(NS::UInteger count)
{
	return Object::sendMessageSafe<bool>(
		this, _MTL_PRIVATE_SEL(supportsVertexAmplificationCount_), count);
}

_MTL_INLINE MTL::SizeAndAlign
			MTL::Device::tensorSizeAndAlign(MTL::TensorDescriptor const* descriptor)
{
	return Object::sendMessage<MTL::SizeAndAlign>(
		this, _MTL_PRIVATE_SEL(tensorSizeAndAlignWithDescriptor_), descriptor);
}
