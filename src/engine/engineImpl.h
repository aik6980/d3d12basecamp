#pragma once

#include "engine.h"

#include "renderer/d3d12/device.h"

class Engine
{
public:
	struct InitData
	{
		HWND	HWnd;
	};

	static const int32_t VERSION = 1;

	int32_t get_version() { return VERSION; }
	
	Engine() {}
	~Engine() {}

	void	Init(const InitData& initData);

	D3D12::Device&	render_device() { return *m_renderDevice; };
private:
	// sub systems;
	unique_ptr<D3D12::Device>	m_renderDevice;
};
