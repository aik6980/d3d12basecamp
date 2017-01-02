#pragma once
#include "stdafx.h"

class RESOURCE_MANAGER
{
public:

private:
	ComPtr<ID3D12Resource> create_default_buffer(ID3D12Device* device,
		ID3D12GraphicsCommandList* command_list,
		UINT64 byte_size,
		const void* init_data,
		ComPtr<ID3D12Resource>& staging_buffer);

	vector<ComPtr<ID3D12Resource>>	m_staging_buffer_list;
};