#include "stdafx.h"

#include "frameResource.h"
#include "engine\engine_cpp.h"

namespace D3D12
{
	FRAME_RESOURCE::FRAME_RESOURCE(ID3D12Device * device)
	{
		DBG::ThrowIfFailed(device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_commandListAlloc.GetAddressOf())));
	}
}