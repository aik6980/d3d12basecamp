#pragma once
#include "stdafx.h"

#include "renderer/renderer.h"

namespace D3D12
{
	class RESOURCE_MANAGER
	{
	public:
		RESOURCE_MANAGER() {}
		~RESOURCE_MANAGER() {}

		ComPtr<ID3D12Resource> create_default_buffer(ID3D12Device& device,
			ID3D12GraphicsCommandList& command_list,
			FRAME_RESOURCE&	frame_resource,
			UINT64 byte_size,
			const void* init_data);

		const MESH_BUFFER*	request_mesh_buffer(const string& str_id);
		bool				register_mesh_buffer(const string& str_id, const MESH_BUFFER& mesh_buffer);
	private:
		ComPtr<ID3D12Resource> create_default_buffer(ID3D12Device& device,
			ID3D12GraphicsCommandList& command_list,
			UINT64 byte_size,
			const void* init_data,
			ComPtr<ID3D12Resource>& staging_buffer);

		unordered_map<string, MESH_BUFFER>	m_mesh_buffer_list;
	};
}