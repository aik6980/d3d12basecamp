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

TEXTURE_DATA TEXTURE_DATA_GENERATOR::create_default_texture(UINT texture_size)
{
	return TEXTURE_DATA();
	/*
	const UINT row_pitch = texture_size * sizeof(XMCOLOR);
	const UINT cellPitch = rowPitch >> 3;		// The width of a cell in the checkboard texture.
	const UINT cellHeight = TextureWidth >> 3;	// The height of a cell in the checkerboard texture.
	const UINT textureSize = rowPitch * TextureHeight;

	std::vector<UINT8> data(textureSize);
	UINT8* pData = &data[0];

	for (UINT n = 0; n < textureSize; n += TexturePixelSize)
	{
		UINT x = n % rowPitch;
		UINT y = n / rowPitch;
		UINT i = x / cellPitch;
		UINT j = y / cellHeight;

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

	return data;
	*/
}
