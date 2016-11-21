#include "stdafx.h"

#include "app.h"
#include "engine/engineImpl.h"

void App::OnInit(HINSTANCE hInstance, HWND hWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	Engine engine;
	Engine::InitData initData;
	initData.HWnd = hWnd;
	engine.Init(initData);
}
