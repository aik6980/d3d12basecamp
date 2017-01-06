#pragma once

#include "device_vulkan.h"

struct GLOBAL
{
	std::unique_ptr<VK::DEVICE>			m_render_device;
	std::unique_ptr<VK::RESOURCE_MGR>	m_render_resource;

	std::unique_ptr<MESH_RENDERER>		m_mesh_renderer;
};

class APP
{
public:
	static GLOBAL&	g() { return m_global; }

	void	on_init(HINSTANCE hInstance, HWND hWnd);
	void	on_update();
	void	on_destroy();
	
	void	on_event_msg(const MSG msg) {};
private:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

	static GLOBAL m_global;
};