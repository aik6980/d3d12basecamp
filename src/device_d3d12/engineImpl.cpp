#include "stdafx.h"

#include "engineImpl.h"
#include "renderer/renderer_cpp.h"
#include "graphic/mesh_renderer.h"

void ENGINE::init(const INIT_DATA& initData)
{
	m_render_device = make_unique<D3D12::DEVICE>();
	m_render_device->LoadPipeline(initData.HWnd);
	m_render_device->OnResize();

	m_resource_mgr = make_unique<D3D12::RESOURCE_MANAGER>();

	m_mesh_renderer = make_unique<MESH_RENDERER>(*this);

	// load rendering resources
	m_render_device->begin_load_resources();

	// ... all subsystem loading resources ...
	m_mesh_renderer->load_resources();

	m_render_device->end_load_resources();
	//////////////////////////////////////////////////////////////////////////
}

void ENGINE::update()
{

}

void ENGINE::draw()
{
	m_render_device->begin_frame();

	// Clear the back buffer and depth buffer.
	// Indicate a state transition on the resource usage.
	m_render_device->commmand_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_render_device->curr_backbuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	m_render_device->commmand_list()->ClearRenderTargetView(m_render_device->curr_backbuffer_view(), DirectX::Colors::LightSteelBlue, 0, nullptr);
	m_render_device->commmand_list()->ClearDepthStencilView(m_render_device->curr_backbuffer_depth_stencil_view(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// draw through a list of renderer
	m_mesh_renderer->draw();

	// Indicate a state transition on the resource usage.
	m_render_device->commmand_list()->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_render_device->curr_backbuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	m_render_device->end_frame();
}
