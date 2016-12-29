#pragma once

#include "renderer/renderer.h"

class ENGINE
{
public:
	struct INIT_DATA
	{
		HWND	HWnd;
	};

	static const int32_t VERSION = 1;

	int32_t get_version() { return VERSION; }

	void	init(const INIT_DATA& initData);
	void	update();
	void	draw();

	D3D12::DEVICE&	render_device() { return *m_render_device; };
private:
	// sub systems;
	unique_ptr<D3D12::DEVICE>	m_render_device;
	unique_ptr<MESH_RENDERER>	m_mesh_renderer;
};
