#pragma once

using Microsoft::WRL::ComPtr;

namespace D3D12
{
	// Stores the resources needed for the CPU to build the command lists
	// for a frame.
	class FRAME_RESOURCE
	{
	public:
		FRAME_RESOURCE(ID3D12Device* device);

		// mark some invalid operation
		FRAME_RESOURCE(const FRAME_RESOURCE& rhs) = delete;
		FRAME_RESOURCE& operator=(const FRAME_RESOURCE& rhs) = delete;

		// destroy all uploaded buffer in this frame
		void clear_upload_buffer_list();
	public:
		// We cannot reset the allocator until the GPU is done processing the commands.
		// So each frame needs their own allocator.
		ComPtr<ID3D12CommandAllocator>	m_command_list_alloc;

		// Fence value to mark commands up to this fence point.  This lets us
		// check if these frame resources are still in use by the GPU.
		UINT64 m_fence = 0;

		// Frame Temporary resources
		vector<ComPtr<ID3D12Resource>>	m_delete_list;
	};
}