#pragma once

class ENGINE;

class APP
{
public:
	void	on_init(HINSTANCE hInstance, HWND hWnd);
	void	on_update();
	void	on_destroy() {};
	
	void	on_event_msg(const MSG msg) {};
private:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;

	// rendering engine
	unique_ptr<ENGINE>	m_engine;
};