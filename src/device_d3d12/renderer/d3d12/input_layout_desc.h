#pragma once
#include "stdafx.h"

#include "input_layout_element_desc_consts.h"

namespace D3D12
{
	struct INPUT_LAYOUT_ELEMENT_DESC : public D3D12_INPUT_ELEMENT_DESC
	{
		INPUT_LAYOUT_ELEMENT_DESC()
		{
			SemanticName = semantic_name_position;
			SemanticIndex = 0;
			Format = DXGI_FORMAT_R32G32B32_FLOAT;
			InputSlot = 0;
			AlignedByteOffset = 0;
			InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			InstanceDataStepRate = 0;
		}
	};

	struct INPUT_LAYOUT_DESC
	{
		vector<INPUT_LAYOUT_ELEMENT_DESC> m_layout;

		D3D12_INPUT_LAYOUT_DESC as_d3d12_input_layout_desc() const;
	};
}