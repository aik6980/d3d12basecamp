#include "stdafx.h"
#include "app.h"

#include "engineImpl.h"
#include "engine_cpp.h"

#include <thread>
#include <atomic>

std::unique_ptr<ENGINE> APP::m_engine;

std::unique_ptr<std::thread> render_thread;
std::atomic<bool> game_running = true;

void APP::on_init(HINSTANCE hInstance, HWND hWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	m_engine = make_unique<ENGINE>();
	ENGINE::INIT_DATA init_data;
	init_data.HWnd = hWnd;
	m_engine->init(init_data);

	// thread
	//m_engine->update();
	//render_thread.reset(new std::thread(
	//	[&]() { while (game_running) { m_engine->draw(); } 
	//}));
}

void APP::on_update()
{
	m_engine->draw();
}

void APP::on_destroy()
{
	game_running = false;
	//render_thread->join();

	m_engine->destroy();
}
