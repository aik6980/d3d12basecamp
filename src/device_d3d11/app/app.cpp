#include "stdafx.h"

#include "app.h"

std::unique_ptr<ENGINE> APP::m_engine;

void APP::on_init(HINSTANCE hInstance, HWND hWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	m_engine = make_unique<ENGINE>();
	//ENGINE::INIT_DATA init_data;
	//init_data.HWnd = hWnd;
	//m_engine->init(init_data);
}

void APP::on_update()
{
	//m_engine->update();
	//m_engine->draw();
}

void APP::on_destroy()
{

}
