#include "stdafx.h"

#include "shader_manager.h"
#include "engine.h"

namespace D3D12
{
	ComPtr<ID3DBlob> SHADER_MANAGER::load_from_objfile(const wstring& filename)
	{
		ComPtr<ID3DBlob> bytecode;
		DBG::ThrowIfFailed(D3DReadFileToBlob(filename.c_str(), &bytecode));

		return bytecode;
	}

}
