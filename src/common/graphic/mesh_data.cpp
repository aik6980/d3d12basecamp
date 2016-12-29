#include "stdafx.h"

#include "mesh_data.h"

void MESH_VERTEX_ARRAY::reset_vertices(UINT n)
{
	m_position.resize(n);
	m_normal.resize(n);
}

void MESH_INDEX_ARRAY::reset_indices(UINT n)
{
	m_indices32.resize(n);
}
