#pragma once
#include "stdafx.h"

#include "d3dx12.h"

namespace D3D12
{
	struct GRAPHICS_PIPELINE_STATE_DESC : public D3D12_GRAPHICS_PIPELINE_STATE_DESC
	{
		GRAPHICS_PIPELINE_STATE_DESC()
		{
			ZeroMemory(this, sizeof(GRAPHICS_PIPELINE_STATE_DESC));
			 
			RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			SampleMask = UINT_MAX;
			PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			SampleDesc.Count = 1;
			SampleDesc.Quality = 0;
		}

		void set_vertex_shader(ID3DBlob* bytecode)
		{
			VS.pShaderBytecode	= bytecode->GetBufferPointer();
			VS.BytecodeLength	= bytecode->GetBufferSize();
		}

		void set_pixel_shader(ID3DBlob* bytecode)
		{
			PS.pShaderBytecode = bytecode->GetBufferPointer();
			PS.BytecodeLength = bytecode->GetBufferSize();
		}
	};
}
