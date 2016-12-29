#pragma once

using DirectX::XMFLOAT3;

struct MESH_VERTEX_ARRAY
{
	// vertices
	vector<XMFLOAT3> m_position;
	vector<XMFLOAT3> m_normal;

	void reset_vertices(UINT n);
};

struct MESH_INDEX_ARRAY
{
	// indices
	vector<uint32_t> m_indices32;

	void reset_indices(UINT n);
};