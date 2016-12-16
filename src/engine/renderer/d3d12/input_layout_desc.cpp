#include "stdafx.h"
#include "input_layout_desc.h"

namespace D3D12
{
	D3D12_INPUT_LAYOUT_DESC INPUT_LAYOUT_DESC::as_d3d12_input_layout_desc() const
	{
		return{ m_layout.data(), (UINT)m_layout.size() };
	}
}

