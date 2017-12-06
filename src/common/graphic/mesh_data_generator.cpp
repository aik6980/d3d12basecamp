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
	vertex_array.m_position[3] = XMFLOAT3( 0.5f, -0.5f, 0.0f);

	index_array.m_indices32[0] = 0;
	index_array.m_indices32[1] = 1;
	index_array.m_indices32[2] = 2;

	index_array.m_indices32[3] = 1;
	index_array.m_indices32[4] = 3;
	index_array.m_indices32[5] = 2;
}


//////////////////////////////////////////////////////////////////////////

TEXTURE_DATA TEXTURE_DATA_GENERATOR::create_default_texture(UINT texture_width, UINT cell_width)
{

	const UINT pixel_size = sizeof(XMCOLOR);
	const UINT row_pitch = texture_width * pixel_size;
	const UINT texture_height = texture_width;
	const UINT cell_pitch = cell_width * pixel_size;		// The width of a cell in the checkboard texture.
	const UINT cell_height = cell_width;	// The height of a cell in the checkerboard texture.
	const UINT texture_size = row_pitch * texture_height;

	// come back and change this to XMCOLOR!
	std::vector<UINT8> data(texture_size);
	UINT8* pData = &data[0];

	for (UINT n = 0; n < texture_size; n += pixel_size)
	{
		UINT x = n % row_pitch;
		UINT y = n / row_pitch;
		UINT i = x / cell_pitch;
		UINT j = y / cell_pitch;

		if (i % 2 == j % 2)
		{
			pData[n] = 0x00;		// R
			pData[n + 1] = 0x00;	// G
			pData[n + 2] = 0x00;	// B
			pData[n + 3] = 0xff;	// A
		}
		else
		{
			pData[n] = 0xff;		// R
			pData[n + 1] = 0xff;	// G
			pData[n + 2] = 0xff;	// B
			pData[n + 3] = 0xff;	// A
		}
	}

	TEXTURE_DATA texture_data;
	texture_data.m_data.resize(texture_width * texture_height);
	memcpy(texture_data.m_data.data(), data.data(), sizeof(data));

	return texture_data;
}
