#pragma once

class App
{
public:
	void	OnInit(HINSTANCE hInstance, HWND hWnd);
	void	OnUpdate();
	void	OnDestroy() {};
	
	void	OnEventMsg(const MSG msg) {};

private:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
};