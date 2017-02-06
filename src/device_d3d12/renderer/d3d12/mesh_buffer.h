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
	};
}