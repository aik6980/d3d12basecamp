#include "stdafx.h"

#include "engineImpl.h"
#include "graphic/mesh_renderer.h"

void ENGINE::init(const INIT_DATA& initData)
{
	m_render_device = make_unique<D3D12::DEVICE>();
	m_render_device->LoadPipeline(initData.HWnd);
	m_render_device->OnResize();

	m_mesh_renderer = make_unique<MESH_RENDERER>(*this);

	// load rendering resources
	m_render_device->begin_load_resources();

	// ... all subsystem loading resources ...

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
	m_render_device->commmand_list()->ClearRenderTargetView(m_render_device->curr_backbuffer_view(), DirectX::Colors::LightSteelBlue, 0, nullptr);
	//m_render_device->commmand_list()->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	m_render_device->end_frame();
}
