#pragma once

#include "stdafx.h"

namespace D3D12
{
	struct MESH_LOCATION
	{
		uint32_t index_count = 0;
		uint32_t start_index_location = 0;
		uint32_t base_vertex_location = 0;
	};

	class MESH_BUFFER
	{
	public:
		ComPtr<ID3D12Resource>	m_vertex_buffer_gpu = nullptr;
		ComPtr<ID3D12Resource>	m_index_buffer_gpu = nullptr;

		unordered_map<string, MESH_LOCATION>	m_mesh_location_list;

		// Data about the buffers.
		uint32_t vtx_bytes_stride = 0;
		uint32_t vb_bytes_size = 0;
		DXGI_FORMAT idx_format = DXGI_FORMAT_R32_UINT;
		uint32_t ib_bytes_size = 0;

		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view() const
		{
			D3D12_VERTEX_BUFFER_VIEW vbv;
			vbv.BufferLocation = m_vertex_buffer_gpu->GetGPUVirtualAddress();
			vbv.SizeInBytes = vb_bytes_size;
			vbv.StrideInBytes = vtx_bytes_stride;

			return vbv;
		}

		D3D12_INDEX_BUFFER_VIEW index_buffer_view() const
		{
			D3D12_INDEX_BUFFER_VIEW ibv;
			ibv.BufferLocation = m_index_buffer_gpu->GetGPUVirtualAddress();
			ibv.Format = idx_format;
			ibv.SizeInBytes = ib_bytes_size;

			return ibv;
		}
	};
}