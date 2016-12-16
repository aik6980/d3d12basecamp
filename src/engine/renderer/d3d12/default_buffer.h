#pragma once
#include "stdafx.h"

namespace D3D12
{
	class DEFAULT_BUFFER
	{
	public: 
		static ComPtr<ID3D12Resource> create_default_buffer();
	};
}