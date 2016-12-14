#include "stdafx.h"

#include "app.h"

#include "engine/engineImpl.h"
#include "engine/graphic/mesh_renderer.h"

void APP::on_init(HINSTANCE hInstance, HWND hWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	m_engine = make_unique<ENGINE>();
	ENGINE::INIT_DATA init_data;
	init_data.HWnd = hWnd;
	m_engine->init(init_data);
}


void APP::on_update()
{
	m_engine->update();

}