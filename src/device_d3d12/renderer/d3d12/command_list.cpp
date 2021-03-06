#include <stdafx.h>

#include "command_list.h"

#include "common/debug/debugUtil.h"
#include "common/debug/debugOutput.h"

void D3D12::COMMAND_LIST::init(ComPtr<ID3D12GraphicsCommandList> com_ptr)
{
	m_obj = com_ptr;
	DBG::ThrowIfFailed(m_obj->Close());
}

void D3D12::COMMAND_LIST::reset(ID3D12CommandAllocator* command_allocator, ID3D12PipelineState* pipeline_state)
{
	if (!m_is_opened)
	{
		DBG::ThrowIfFailed(m_obj->Reset(command_allocator, pipeline_state));
		m_is_opened = true;
	}
}

void D3D12::COMMAND_LIST::close()
{
	if (m_is_opened)
	{
		DBG::ThrowIfFailed(m_obj->Close());
		m_is_opened = false;
	}
}
