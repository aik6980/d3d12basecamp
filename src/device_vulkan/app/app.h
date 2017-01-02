#pragma once

namespace VK
{
	class DEVICE;
}

class APP
{
public:
	void	on_init(HINSTANCE hInstance, HWND hWnd);
	void	on_update();
	void	on_destroy();
	
	void	on_event_msg(const MSG msg) {};
private:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

	std::unique_ptr<VK::DEVICE>		m_render_device;
};