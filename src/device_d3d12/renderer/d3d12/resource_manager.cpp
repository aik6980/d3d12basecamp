#include "stdafx.h"

#include "resource_manager.h"
#include "common/common.cpp.h"
#include "renderer/renderer_cpp.h"

namespace D3D12
{
	ComPtr<ID3D12Resource> RESOURCE_MANAGER::create_default_buffer(ID3D12Device& device,
		ID3D12GraphicsCommandList& command_list, FRAME_RESOURCE& frame_resource, UINT64 byte_size, const void* init_data)
	{
		ComPtr<ID3D12Resource> resource;
		ComPtr<ID3D12Resource> staging_buffer;
		// if we need a staging buffer, create one then add to FRAME_RESOURCE delete list
		resource = create_default_buffer(device, command_list, byte_size, init_data, staging_buffer);
		if (init_data)
		{
			frame_resource.m_delete_list.push_back(staging_buffer);
		}

		return resource;
	}

	ComPtr<ID3D12Resource> RESOURCE_MANAGER::create_default_buffer(ID3D12Device& device,
		ID3D12GraphicsCommandList& command_list, UINT64 byte_size,
		const void* init_data, ComPtr<ID3D12Resource>& staging_buffer)
	{
		ComPtr<ID3D12Resource> default_buffer;

		DBG::ThrowIfFailed(device.CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(byte_size),
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&default_buffer)));

		// optional, if we want to upload an initial_data 
		if (init_data)
		{
			// staging buffer, in order to copy CPU memory data into our default buffer, we need to create
			// an intermediate upload heap.
			DBG::ThrowIfFailed(device.CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(byte_size),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&staging_buffer)));

			// Describe the data we want to copy into the default buffer.
			D3D12_SUBRESOURCE_DATA sub_resource_data = {};
			sub_resource_data.pData = init_data;
			sub_resource_data.RowPitch = byte_size;
			sub_resource_data.SlicePitch = sub_resource_data.RowPitch;

			// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
			// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
			// the intermediate upload heap data will be copied to mBuffer.
			command_list.ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(default_buffer.Get(),
				D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

			// [note] this function is not at the lowest level either, 
			// so, still room to learn down there
			UpdateSubresources<1>(&command_list, default_buffer.Get(), staging_buffer.Get(), 0, 0, 1, &sub_resource_data);

			command_list.ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(default_buffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
		}

		return default_buffer;
	}

	const MESH_BUFFER* RESOURCE_MANAGER::request_mesh_buffer(const string& str_id)
	{
		auto&& itr = m_mesh_buffer_list.find(str_id);
		if (itr != m_mesh_buffer_list.end())
		{
			return &itr->second;
		}
		return nullptr;
	}

	bool RESOURCE_MANAGER::register_mesh_buffer(const string& str_id, const MESH_BUFFER& mesh_buffer)
	{
		auto&& m = request_mesh_buffer(str_id);
		if (!m)
		{
			m_mesh_buffer_list.insert(make_pair(str_id, mesh_buffer));
			return true;
		}
		return false;
	}
}