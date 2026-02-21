//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// Metal/MTL4Compiler.hpp
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
#include "MTLDefines.hpp"
#include "MTLDevice.hpp"
#include "MTLHeaderBridge.hpp"
#include "MTLPrivate.hpp"

#include <Helpers/Function.hpp>

namespace MTL4
{
	class BinaryFunction;
	class BinaryFunctionDescriptor;
	class CompilerDescriptor;
	class CompilerTask;
	class CompilerTaskOptions;
	class ComputePipelineDescriptor;
	class LibraryDescriptor;
	class MachineLearningPipelineDescriptor;
	class MachineLearningPipelineState;
	class PipelineDataSetSerializer;
	class PipelineDescriptor;
	class PipelineStageDynamicLinkingDescriptor;
	class RenderPipelineDynamicLinkingDescriptor;
}

namespace MTL
{
	class ComputePipelineState;
	class Device;
	class DynamicLibrary;
	class Library;
	class RenderPipelineState;

	using NewDynamicLibraryCompletionHandler = void (^)(MTL::DynamicLibrary*, NS::Error*);
	using NewDynamicLibraryCompletionHandlerFunction =
		MtlCpp::Function<void(MTL::DynamicLibrary*, NS::Error*)>;
}

namespace MTL4
{
	using NewComputePipelineStateCompletionHandler = void (^)(MTL::ComputePipelineState*,
	                                                          NS::Error*);
	using NewComputePipelineStateCompletionHandlerFunction =
		MtlCpp::Function<void(MTL::ComputePipelineState*, NS::Error*)>;
	using NewRenderPipelineStateCompletionHandler = void (^)(MTL::RenderPipelineState*,
	                                                         NS::Error*);
	using NewRenderPipelineStateCompletionHandlerFunction =
		MtlCpp::Function<void(MTL::RenderPipelineState*, NS::Error*)>;
	using NewBinaryFunctionCompletionHandler = void (^)(MTL4::BinaryFunction*,
	                                                    NS::Error*);
	using NewBinaryFunctionCompletionHandlerFunction =
		MtlCpp::Function<void(MTL4::BinaryFunction*, NS::Error*)>;
	using NewMachineLearningPipelineStateCompletionHandler =
		void (^)(MTL4::MachineLearningPipelineState*, NS::Error*);
	using NewMachineLearningPipelineStateCompletionHandlerFunction =
		MtlCpp::Function<void(MTL4::MachineLearningPipelineState*, NS::Error*)>;

	class CompilerDescriptor : public NS::Copying<CompilerDescriptor>
	{
	public:
		static CompilerDescriptor* alloc();

		CompilerDescriptor* init();

		NS::String* label() const;

		PipelineDataSetSerializer* pipelineDataSetSerializer() const;

		void setLabel(NS::String const* label);

		void setPipelineDataSetSerializer(
			MTL4::PipelineDataSetSerializer const* pipelineDataSetSerializer);
	};

	class CompilerTaskOptions : public NS::Copying<CompilerTaskOptions>
	{
	public:
		static CompilerTaskOptions* alloc();

		CompilerTaskOptions* init();

		NS::Array* lookupArchives() const;
		void       setLookupArchives(NS::Array const* lookupArchives);
	};

	class Compiler : public NS::Referencing<Compiler>
	{
	public:
		MTL::Device* device() const;

		NS::String* label() const;

		BinaryFunction* newBinaryFunction(
			MTL4::BinaryFunctionDescriptor const* descriptor,
			MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error);
		CompilerTask* newBinaryFunction(
			MTL4::BinaryFunctionDescriptor const*          descriptor,
			MTL4::CompilerTaskOptions const*               compilerTaskOptions,
			MTL4::NewBinaryFunctionCompletionHandler const completionHandler);

		MTL::ComputePipelineState* newComputePipelineState(
			MTL4::ComputePipelineDescriptor const* descriptor,
			MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error);
		MTL::ComputePipelineState* newComputePipelineState(
			MTL4::ComputePipelineDescriptor const*             descriptor,
			MTL4::PipelineStageDynamicLinkingDescriptor const* dynamicLinkingDescriptor,
			MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error);
		CompilerTask* newComputePipelineState(
			MTL4::ComputePipelineDescriptor const*              descriptor,
			MTL4::CompilerTaskOptions const*                    compilerTaskOptions,
			MTL::NewComputePipelineStateCompletionHandler const completionHandler);
		CompilerTask* newComputePipelineState(
			MTL4::ComputePipelineDescriptor const*              descriptor,
			MTL4::PipelineStageDynamicLinkingDescriptor const*  dynamicLinkingDescriptor,
			MTL4::CompilerTaskOptions const*                    compilerTaskOptions,
			MTL::NewComputePipelineStateCompletionHandler const completionHandler);
		CompilerTask* newComputePipelineState(
			MTL4::ComputePipelineDescriptor const*                        pDescriptor,
			MTL4::CompilerTaskOptions const*                              options,
			MTL4::NewComputePipelineStateCompletionHandlerFunction const& function);

		MTL::DynamicLibrary* newDynamicLibrary(MTL::Library const* library,
		                                       NS::Error**         error);
		MTL::DynamicLibrary* newDynamicLibrary(NS::URL const* url, NS::Error** error);
		CompilerTask*        newDynamicLibrary(
				   MTL::Library const*                           library,
				   MTL::NewDynamicLibraryCompletionHandler const completionHandler);
		CompilerTask* newDynamicLibrary(
			NS::URL const*                                url,
			MTL::NewDynamicLibraryCompletionHandler const completionHandler);
		CompilerTask* newDynamicLibrary(
			MTL::Library const*                                    pLibrary,
			MTL::NewDynamicLibraryCompletionHandlerFunction const& function);
		CompilerTask* newDynamicLibrary(
			NS::URL const*                                         pURL,
			MTL::NewDynamicLibraryCompletionHandlerFunction const& function);

		MTL::Library* newLibrary(MTL4::LibraryDescriptor const* descriptor,
		                         NS::Error**                    error);
		CompilerTask*
		newLibrary(MTL4::LibraryDescriptor const*         descriptor,
		           MTL::NewLibraryCompletionHandler const completionHandler);
		CompilerTask*
		newLibrary(MTL4::LibraryDescriptor const*                  pDescriptor,
		           MTL::NewLibraryCompletionHandlerFunction const& function);

		MachineLearningPipelineState* newMachineLearningPipelineState(
			MTL4::MachineLearningPipelineDescriptor const* descriptor, NS::Error** error);
		CompilerTask* newMachineLearningPipelineState(
			MTL4::MachineLearningPipelineDescriptor const* descriptor,
			MTL4::NewMachineLearningPipelineStateCompletionHandler const
				completionHandler);
		CompilerTask* newMachineLearningPipelineState(
			MTL4::MachineLearningPipelineDescriptor const* pDescriptor,
			MTL4::NewMachineLearningPipelineStateCompletionHandlerFunction const&
				function);

		MTL::RenderPipelineState* newRenderPipelineState(
			MTL4::PipelineDescriptor const*  descriptor,
			MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error);
		MTL::RenderPipelineState* newRenderPipelineState(
			MTL4::PipelineDescriptor const*                     descriptor,
			MTL4::RenderPipelineDynamicLinkingDescriptor const* dynamicLinkingDescriptor,
			MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error);
		CompilerTask* newRenderPipelineState(
			MTL4::PipelineDescriptor const*                    descriptor,
			MTL4::CompilerTaskOptions const*                   compilerTaskOptions,
			MTL::NewRenderPipelineStateCompletionHandler const completionHandler);
		CompilerTask* newRenderPipelineState(
			MTL4::PipelineDescriptor const*                     descriptor,
			MTL4::RenderPipelineDynamicLinkingDescriptor const* dynamicLinkingDescriptor,
			MTL4::CompilerTaskOptions const*                    compilerTaskOptions,
			MTL::NewRenderPipelineStateCompletionHandler const  completionHandler);
		CompilerTask* newRenderPipelineState(
			MTL4::PipelineDescriptor const*                              pDescriptor,
			MTL4::CompilerTaskOptions const*                             options,
			MTL4::NewRenderPipelineStateCompletionHandlerFunction const& function);
		MTL::RenderPipelineState* newRenderPipelineStateBySpecialization(
			MTL4::PipelineDescriptor const* descriptor,
			MTL::RenderPipelineState const* pipeline, NS::Error** error);
		CompilerTask* newRenderPipelineStateBySpecialization(
			MTL4::PipelineDescriptor const*                    descriptor,
			MTL::RenderPipelineState const*                    pipeline,
			MTL::NewRenderPipelineStateCompletionHandler const completionHandler);
		CompilerTask* newRenderPipelineStateBySpecialization(
			MTL4::PipelineDescriptor const*                              pDescriptor,
			MTL::RenderPipelineState const*                              pPipeline,
			MTL4::NewRenderPipelineStateCompletionHandlerFunction const& function);

		PipelineDataSetSerializer* pipelineDataSetSerializer() const;
	};

}

_MTL_INLINE MTL4::CompilerDescriptor* MTL4::CompilerDescriptor::alloc()
{
	return NS::Object::alloc<MTL4::CompilerDescriptor>(
		_MTL_PRIVATE_CLS(MTL4CompilerDescriptor));
}

_MTL_INLINE MTL4::CompilerDescriptor* MTL4::CompilerDescriptor::init()
{
	return NS::Object::init<MTL4::CompilerDescriptor>();
}

_MTL_INLINE NS::String* MTL4::CompilerDescriptor::label() const
{
	return Object::sendMessage<NS::String*>(this, _MTL_PRIVATE_SEL(label));
}

_MTL_INLINE MTL4::PipelineDataSetSerializer*
			MTL4::CompilerDescriptor::pipelineDataSetSerializer() const
{
	return Object::sendMessage<MTL4::PipelineDataSetSerializer*>(
		this, _MTL_PRIVATE_SEL(pipelineDataSetSerializer));
}

_MTL_INLINE void MTL4::CompilerDescriptor::setLabel(NS::String const* label)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setLabel_), label);
}

_MTL_INLINE void MTL4::CompilerDescriptor::setPipelineDataSetSerializer(
	MTL4::PipelineDataSetSerializer const* pipelineDataSetSerializer)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setPipelineDataSetSerializer_),
	                          pipelineDataSetSerializer);
}

_MTL_INLINE MTL4::CompilerTaskOptions* MTL4::CompilerTaskOptions::alloc()
{
	return NS::Object::alloc<MTL4::CompilerTaskOptions>(
		_MTL_PRIVATE_CLS(MTL4CompilerTaskOptions));
}

_MTL_INLINE MTL4::CompilerTaskOptions* MTL4::CompilerTaskOptions::init()
{
	return NS::Object::init<MTL4::CompilerTaskOptions>();
}

_MTL_INLINE NS::Array* MTL4::CompilerTaskOptions::lookupArchives() const
{
	return Object::sendMessage<NS::Array*>(this, _MTL_PRIVATE_SEL(lookupArchives));
}

_MTL_INLINE void
MTL4::CompilerTaskOptions::setLookupArchives(NS::Array const* lookupArchives)
{
	Object::sendMessage<void>(this, _MTL_PRIVATE_SEL(setLookupArchives_), lookupArchives);
}

_MTL_INLINE MTL::Device* MTL4::Compiler::device() const
{
	return Object::sendMessage<MTL::Device*>(this, _MTL_PRIVATE_SEL(device));
}

_MTL_INLINE NS::String* MTL4::Compiler::label() const
{
	return Object::sendMessage<NS::String*>(this, _MTL_PRIVATE_SEL(label));
}

_MTL_INLINE MTL4::BinaryFunction* MTL4::Compiler::newBinaryFunction(
	MTL4::BinaryFunctionDescriptor const* descriptor,
	MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error)
{
	return Object::sendMessage<MTL4::BinaryFunction*>(
		this,
		_MTL_PRIVATE_SEL(newBinaryFunctionWithDescriptor_compilerTaskOptions_error_),
		descriptor, compilerTaskOptions, error);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newBinaryFunction(
	MTL4::BinaryFunctionDescriptor const*          descriptor,
	MTL4::CompilerTaskOptions const*               compilerTaskOptions,
	MTL4::NewBinaryFunctionCompletionHandler const completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this,
		_MTL_PRIVATE_SEL(
			newBinaryFunctionWithDescriptor_compilerTaskOptions_completionHandler_),
		descriptor, compilerTaskOptions, completionHandler);
}

_MTL_INLINE MTL::ComputePipelineState* MTL4::Compiler::newComputePipelineState(
	MTL4::ComputePipelineDescriptor const* descriptor,
	MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error)
{
	return Object::sendMessage<MTL::ComputePipelineState*>(
		this,
		_MTL_PRIVATE_SEL(
			newComputePipelineStateWithDescriptor_compilerTaskOptions_error_),
		descriptor, compilerTaskOptions, error);
}

_MTL_INLINE MTL::ComputePipelineState* MTL4::Compiler::newComputePipelineState(
	MTL4::ComputePipelineDescriptor const*             descriptor,
	MTL4::PipelineStageDynamicLinkingDescriptor const* dynamicLinkingDescriptor,
	MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error)
{
	return Object::sendMessage<MTL::ComputePipelineState*>(
		this,
		_MTL_PRIVATE_SEL(
			newComputePipelineStateWithDescriptor_dynamicLinkingDescriptor_compilerTaskOptions_error_),
		descriptor, dynamicLinkingDescriptor, compilerTaskOptions, error);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newComputePipelineState(
	MTL4::ComputePipelineDescriptor const*              descriptor,
	MTL4::CompilerTaskOptions const*                    compilerTaskOptions,
	MTL::NewComputePipelineStateCompletionHandler const completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this,
		_MTL_PRIVATE_SEL(
			newComputePipelineStateWithDescriptor_compilerTaskOptions_completionHandler_),
		descriptor, compilerTaskOptions, completionHandler);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newComputePipelineState(
	MTL4::ComputePipelineDescriptor const*              descriptor,
	MTL4::PipelineStageDynamicLinkingDescriptor const*  dynamicLinkingDescriptor,
	MTL4::CompilerTaskOptions const*                    compilerTaskOptions,
	MTL::NewComputePipelineStateCompletionHandler const completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this,
		_MTL_PRIVATE_SEL(
			newComputePipelineStateWithDescriptor_dynamicLinkingDescriptor_compilerTaskOptions_completionHandler_),
		descriptor, dynamicLinkingDescriptor, compilerTaskOptions, completionHandler);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newComputePipelineState(
	MTL4::ComputePipelineDescriptor const*                        pDescriptor,
	MTL4::CompilerTaskOptions const*                              options,
	MTL4::NewComputePipelineStateCompletionHandlerFunction const& function)
{
	__block MTL4::NewComputePipelineStateCompletionHandlerFunction blockFunction =
		function;
	return newComputePipelineState(
		pDescriptor, options, ^(MTL::ComputePipelineState* pPipeline, NS::Error* pError) {
		  blockFunction(pPipeline, pError);
		});
}

_MTL_INLINE MTL::DynamicLibrary*
MTL4::Compiler::newDynamicLibrary(MTL::Library const* library, NS::Error** error)
{
	return Object::sendMessage<MTL::DynamicLibrary*>(
		this, _MTL_PRIVATE_SEL(newDynamicLibrary_error_), library, error);
}

_MTL_INLINE MTL::DynamicLibrary* MTL4::Compiler::newDynamicLibrary(NS::URL const* url,
                                                                   NS::Error**    error)
{
	return Object::sendMessage<MTL::DynamicLibrary*>(
		this, _MTL_PRIVATE_SEL(newDynamicLibraryWithURL_error_), url, error);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newDynamicLibrary(
	MTL::Library const*                           library,
	MTL::NewDynamicLibraryCompletionHandler const completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this, _MTL_PRIVATE_SEL(newDynamicLibrary_completionHandler_), library,
		completionHandler);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newDynamicLibrary(
	NS::URL const* url, MTL::NewDynamicLibraryCompletionHandler const completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this, _MTL_PRIVATE_SEL(newDynamicLibraryWithURL_completionHandler_), url,
		completionHandler);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newDynamicLibrary(
	MTL::Library const*                                    pLibrary,
	MTL::NewDynamicLibraryCompletionHandlerFunction const& function)
{
	__block MTL::NewDynamicLibraryCompletionHandlerFunction blockFunction = function;
	return newDynamicLibrary(pLibrary,
	                         ^(MTL::DynamicLibrary* pLibraryRef, NS::Error* pError) {
							   blockFunction(pLibraryRef, pError);
							 });
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newDynamicLibrary(
	NS::URL const* pURL, MTL::NewDynamicLibraryCompletionHandlerFunction const& function)
{
	__block MTL::NewDynamicLibraryCompletionHandlerFunction blockFunction = function;
	return newDynamicLibrary(pURL, ^(MTL::DynamicLibrary* pLibrary, NS::Error* pError) {
	  blockFunction(pLibrary, pError);
	});
}

_MTL_INLINE MTL::Library*
MTL4::Compiler::newLibrary(MTL4::LibraryDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL::Library*>(
		this, _MTL_PRIVATE_SEL(newLibraryWithDescriptor_error_), descriptor, error);
}

_MTL_INLINE MTL4::CompilerTask*
			MTL4::Compiler::newLibrary(MTL4::LibraryDescriptor const*         descriptor,
                                       MTL::NewLibraryCompletionHandler const completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this, _MTL_PRIVATE_SEL(newLibraryWithDescriptor_completionHandler_), descriptor,
		completionHandler);
}

_MTL_INLINE MTL4::CompilerTask*
			MTL4::Compiler::newLibrary(MTL4::LibraryDescriptor const*                  pDescriptor,
                                       MTL::NewLibraryCompletionHandlerFunction const& function)
{
	__block MTL::NewLibraryCompletionHandlerFunction blockFunction = function;
	return newLibrary(pDescriptor, ^(MTL::Library* pLibrary, NS::Error* pError) {
	  blockFunction(pLibrary, pError);
	});
}

_MTL_INLINE MTL4::MachineLearningPipelineState*
			MTL4::Compiler::newMachineLearningPipelineState(
    MTL4::MachineLearningPipelineDescriptor const* descriptor, NS::Error** error)
{
	return Object::sendMessage<MTL4::MachineLearningPipelineState*>(
		this, _MTL_PRIVATE_SEL(newMachineLearningPipelineStateWithDescriptor_error_),
		descriptor, error);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newMachineLearningPipelineState(
	MTL4::MachineLearningPipelineDescriptor const*               descriptor,
	MTL4::NewMachineLearningPipelineStateCompletionHandler const completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this,
		_MTL_PRIVATE_SEL(
			newMachineLearningPipelineStateWithDescriptor_completionHandler_),
		descriptor, completionHandler);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newMachineLearningPipelineState(
	MTL4::MachineLearningPipelineDescriptor const*                        pDescriptor,
	MTL4::NewMachineLearningPipelineStateCompletionHandlerFunction const& function)
{
	__block MTL4::NewMachineLearningPipelineStateCompletionHandlerFunction blockFunction =
		function;
	return newMachineLearningPipelineState(
		pDescriptor, ^(MTL4::MachineLearningPipelineState* pPipeline, NS::Error* pError) {
		  blockFunction(pPipeline, pError);
		});
}

_MTL_INLINE MTL::RenderPipelineState* MTL4::Compiler::newRenderPipelineState(
	MTL4::PipelineDescriptor const*  descriptor,
	MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error)
{
	return Object::sendMessage<MTL::RenderPipelineState*>(
		this,
		_MTL_PRIVATE_SEL(newRenderPipelineStateWithDescriptor_compilerTaskOptions_error_),
		descriptor, compilerTaskOptions, error);
}

_MTL_INLINE MTL::RenderPipelineState* MTL4::Compiler::newRenderPipelineState(
	MTL4::PipelineDescriptor const*                     descriptor,
	MTL4::RenderPipelineDynamicLinkingDescriptor const* dynamicLinkingDescriptor,
	MTL4::CompilerTaskOptions const* compilerTaskOptions, NS::Error** error)
{
	return Object::sendMessage<MTL::RenderPipelineState*>(
		this,
		_MTL_PRIVATE_SEL(
			newRenderPipelineStateWithDescriptor_dynamicLinkingDescriptor_compilerTaskOptions_error_),
		descriptor, dynamicLinkingDescriptor, compilerTaskOptions, error);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newRenderPipelineState(
	MTL4::PipelineDescriptor const*                    descriptor,
	MTL4::CompilerTaskOptions const*                   compilerTaskOptions,
	MTL::NewRenderPipelineStateCompletionHandler const completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this,
		_MTL_PRIVATE_SEL(
			newRenderPipelineStateWithDescriptor_compilerTaskOptions_completionHandler_),
		descriptor, compilerTaskOptions, completionHandler);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newRenderPipelineState(
	MTL4::PipelineDescriptor const*                     descriptor,
	MTL4::RenderPipelineDynamicLinkingDescriptor const* dynamicLinkingDescriptor,
	MTL4::CompilerTaskOptions const*                    compilerTaskOptions,
	MTL::NewRenderPipelineStateCompletionHandler const  completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this,
		_MTL_PRIVATE_SEL(
			newRenderPipelineStateWithDescriptor_dynamicLinkingDescriptor_compilerTaskOptions_completionHandler_),
		descriptor, dynamicLinkingDescriptor, compilerTaskOptions, completionHandler);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newRenderPipelineState(
	MTL4::PipelineDescriptor const* pDescriptor, MTL4::CompilerTaskOptions const* options,
	MTL4::NewRenderPipelineStateCompletionHandlerFunction const& function)
{
	__block MTL4::NewRenderPipelineStateCompletionHandlerFunction blockFunction =
		function;
	return newRenderPipelineState(
		pDescriptor, options, ^(MTL::RenderPipelineState* pPipeline, NS::Error* pError) {
		  blockFunction(pPipeline, pError);
		});
}

_MTL_INLINE MTL::RenderPipelineState*
			MTL4::Compiler::newRenderPipelineStateBySpecialization(
    MTL4::PipelineDescriptor const* descriptor, MTL::RenderPipelineState const* pipeline,
    NS::Error** error)
{
	return Object::sendMessage<MTL::RenderPipelineState*>(
		this,
		_MTL_PRIVATE_SEL(
			newRenderPipelineStateBySpecializationWithDescriptor_pipeline_error_),
		descriptor, pipeline, error);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newRenderPipelineStateBySpecialization(
	MTL4::PipelineDescriptor const* descriptor, MTL::RenderPipelineState const* pipeline,
	MTL::NewRenderPipelineStateCompletionHandler const completionHandler)
{
	return Object::sendMessage<MTL4::CompilerTask*>(
		this,
		_MTL_PRIVATE_SEL(
			newRenderPipelineStateBySpecializationWithDescriptor_pipeline_completionHandler_),
		descriptor, pipeline, completionHandler);
}

_MTL_INLINE MTL4::CompilerTask* MTL4::Compiler::newRenderPipelineStateBySpecialization(
	MTL4::PipelineDescriptor const*                              pDescriptor,
	MTL::RenderPipelineState const*                              pPipeline,
	MTL4::NewRenderPipelineStateCompletionHandlerFunction const& function)
{
	__block MTL4::NewRenderPipelineStateCompletionHandlerFunction blockFunction =
		function;
	return newRenderPipelineStateBySpecialization(
		pDescriptor, pPipeline,
		^(MTL::RenderPipelineState* pPipelineRef, NS::Error* pError) {
		  blockFunction(pPipelineRef, pError);
		});
}

_MTL_INLINE MTL4::PipelineDataSetSerializer*
			MTL4::Compiler::pipelineDataSetSerializer() const
{
	return Object::sendMessage<MTL4::PipelineDataSetSerializer*>(
		this, _MTL_PRIVATE_SEL(pipelineDataSetSerializer));
}
