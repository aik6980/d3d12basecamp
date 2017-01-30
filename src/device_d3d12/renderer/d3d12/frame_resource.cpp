#include "stdafx.h"

#include "frame_resource.h"
#include "engine_cpp.h"

namespace D3D12
{
	FRAME_RESOURCE::FRAME_RESOURCE(ID3D12Device * device)
	{
		DBG::ThrowIfFailed(device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_command_list_alloc.GetAddressOf())));
	}
}