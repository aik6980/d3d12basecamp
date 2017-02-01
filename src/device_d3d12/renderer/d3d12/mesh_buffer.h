#pragma once

#include "stdafx.h"

struct MESH_LOCATION
{
	uint32_t index_count = 0;
	uint32_t start_index_location = 0;
	uint32_t base_vertex_location = 0;
};

class MESH_BUFFER
{
	string m_name;
};