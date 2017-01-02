#include "stdafx.h"
#include "app.h"

#include "device_vulkan.cpp.h"

void APP::on_init(HINSTANCE hInstance, HWND hWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	m_render_device.reset(new VK::DEVICE());
	VK::DEVICE::INIT_DATA init_info = { m_hInstance, m_hWnd };
	m_render_device->init(init_info);
}

void APP::on_update()
{

}

void APP::on_destroy()
{
	m_render_device->destroy();
}