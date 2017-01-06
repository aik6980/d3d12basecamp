#include "stdafx.h"
#include "app.h"

#include "device_vulkan.cpp.h"

GLOBAL APP::m_global;

void APP::on_init(HINSTANCE hInstance, HWND hWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	g().m_render_device.reset(new VK::DEVICE());
	VK::DEVICE::INIT_DATA init_info = { m_hInstance, m_hWnd };
	g().m_render_device->init(init_info);

	g().m_render_resource.reset(new VK::RESOURCE_MGR(*g().m_render_device));

	g().m_mesh_renderer.reset(new MESH_RENDERER());
}

void APP::on_update()
{
	g().m_mesh_renderer->update();
}

void APP::on_destroy()
{
	g().m_render_device->destroy();
}
