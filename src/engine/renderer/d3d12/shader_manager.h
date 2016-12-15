#pragma once

#include "stdafx.h"

namespace D3D12
{
	class SHADER_MANAGER
	{
	public:
		static ComPtr<ID3DBlob> load_from_objfile(const wstring& filename);
	};
}