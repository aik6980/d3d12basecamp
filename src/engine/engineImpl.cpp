#include "stdafx.h"

#include "engineImpl.h"

void Engine::Init(const InitData& initData)
{
	m_renderDevice = make_unique<D3D12::Device>();
	m_renderDevice->LoadPipeline(initData.HWnd);
}
