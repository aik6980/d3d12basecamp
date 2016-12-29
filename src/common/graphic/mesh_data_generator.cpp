#include "stdafx.h"

#include "mesh_data_generator.h"
#include "graphic_cpp.h"

void MESH_DATA_GENERATOR::create_unit_quad(MESH_VERTEX_ARRAY& vertex_array, MESH_INDEX_ARRAY& index_array)
{
	vertex_array.reset_vertices(4);
	index_array.reset_indices(6);

	vertex_array.m_position[0] = XMFLOAT3(-0.5f,  0.5f, 0.0f);
	vertex_array.m_position[1] = XMFLOAT3( 0.5f,  0.5f, 0.0f);
	vertex_array.m_position[2] = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertex_array.m_position[3] = XMFLOAT3(-0.5f,  0.5f, 0.0f);

	index_array.m_indices32[0] = 0;
	index_array.m_indices32[1] = 1;
	index_array.m_indices32[2] = 2;

	index_array.m_indices32[3] = 1;
	index_array.m_indices32[4] = 3;
	index_array.m_indices32[5] = 2;
}

