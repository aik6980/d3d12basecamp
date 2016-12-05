#pragma once

using Microsoft::WRL::ComPtr;

namespace D3D12
{
	// Stores the resources needed for the CPU to build the command lists
	// for a frame.
	class FrameResource
	{
		// mark some invalid operation
		FrameResource(const FrameResource& rhs) = delete;
		FrameResource& operator=(const FrameResource& rhs) = delete;



		// We cannot reset the allocator until the GPU is done processing the commands.
		// So each frame needs their own allocator.
		ComPtr<ID3D12CommandAllocator>	m_commandListAlloc;

		// Fence value to mark commands up to this fence point.  This lets us
		// check if these frame resources are still in use by the GPU.
		UINT64 m_fence = 0;
	};
}