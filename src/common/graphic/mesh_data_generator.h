#pragma once

#include "graphic.h"

class MESH_DATA_GENERATOR
{
public:
	static void create_unit_quad(MESH_VERTEX_ARRAY& vertex_array, MESH_INDEX_ARRAY& index_array);
};

class TEXTURE_DATA_GENERATOR
{
public:
	static TEXTURE_DATA create_default_texture(UINT texture_size);
};