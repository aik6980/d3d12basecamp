#include "stdafx.h"

#include "device.h"

#include "engine/core/debug/debugUtil.h"
#include "engine/core/debug/debugOutput.h"

namespace D3D12
{
	static const D3D_FEATURE_LEVEL REQUESTED_FEATURE_LEVEL = D3D_FEATURE_LEVEL_12_1;

	void Device::LoadPipeline(HWND hwnd)
	{
		m_hWnd = hwnd;

		// enable debug layer
		{
			ComPtr<ID3D12Debug> dbgController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dbgController))))
			{
				dbgController->EnableDebugLayer();
			}
		}

		ComPtr<IDXGIFactory4> factory;
		DBG::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

		FindHardwareAdapter(*factory.Get());
		if (!m_adapter.Get())
		{
			DBG::OutputString("Failed to Find D3D12 Supported HW, Creating SW Device...");
			factory->EnumWarpAdapter(IID_PPV_ARGS(&m_adapter));
		}
		m_dxgiFactory = factory;


		// create device
		DBG::ThrowIfFailed(D3D12CreateDevice(m_adapter.Get(), REQUESTED_FEATURE_LEVEL, IID_PPV_ARGS(&m_device)));

		// create fence
		DBG::ThrowIfFailed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

		// query RTV and DSV size from driver
		m_rtvDescHeapSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		m_dsvDescHeapSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		m_cbvSrvUavDescHeapSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// create a graphic command list
		CreateCommandObjects();
		// create a swap chain
		CreateSwapChain();
		// create rtv and dsv heap
		CreateRtvAndDsvDescriptorHeaps();
	}

	void Device::OnResize()
	{
		assert(m_device);
		assert(m_swapChain);
		assert(m_commandAlloc);

		// Flush before changing any resources.
		FlushCommmandQueue();

		DBG::ThrowIfFailed(m_commandList->Reset(m_commandAlloc.Get(), nullptr));

		// Release the previous resources we will be recreating.
		for (auto&& a : m_swapChainBuffer)
		{
			a.Reset();
		}
		m_depthBuffer.Reset();


		CRect clientRect;
		GetClientRect(m_hWnd, &clientRect);
		// Resize the swap chain.
		DBG::ThrowIfFailed(m_swapChain->ResizeBuffers(m_frameCount, clientRect.Width(), clientRect.Height(), DXGI_FORMAT_R8G8B8A8_UNORM, ))
	}

	void Device::FlushCommmandQueue()
	{
		// Advance the fence value to mark commands up to this fence point.
		m_currentFence++;

		// Add an instruction to the command queue to set a new fence point.  Because we 
		// are on the GPU timeline, the new fence point won't be set until the GPU finishes
		// processing all the commands prior to this Signal().
		DBG::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_currentFence));

		// Wait until the GPU has completed commands up to this fence point.
		if (m_fence->GetCompletedValue() < m_currentFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

			// Fire event when GPU hits current fence.
			DBG::ThrowIfFailed(m_fence->SetEventOnCompletion(m_currentFence, eventHandle));

			// Wait until the GPU hits current fence event is fired.
			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}

	void Device::FindHardwareAdapter(IDXGIFactory4& factory)
	{
		ComPtr<IDXGIAdapter1> adapter;

		for (uint32_t i = 0; DXGI_ERROR_NOT_FOUND != factory.EnumAdapters1(i, &adapter); ++i)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

			// test D3D12 device creation, no actual device created yet
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), REQUESTED_FEATURE_LEVEL, __uuidof(ID3D12Device), nullptr)))
			{
				DBG::OutputString(L"Found D3D12 HW, %s \n", desc.Description);
				break;
			}
		}

		m_adapter = adapter;
	}

	void Device::CreateCommandObjects()
	{
		D3D12_COMMAND_QUEUE_DESC desc;
		desc.Type	= D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		DBG::ThrowIfFailed(m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_commandQueue)));

		DBG::ThrowIfFailed(m_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_commandAlloc)));

		DBG::ThrowIfFailed(m_device->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			m_commandAlloc.Get(),
			nullptr,
			IID_PPV_ARGS(&m_commandList)));

		// Start off in a closed state.  This is because the first time we refer 
		// to the command list we will Reset it, and it needs to be closed before
		// calling Reset.
		m_commandList->Close();
	}

	void Device::CreateSwapChain()
	{
		CRect clientRect;
		GetClientRect(m_hWnd, &clientRect);

		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferCount = m_frameCount;
		desc.BufferDesc.RefreshRate.Numerator = 60; // Target FPS
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Width = clientRect.Width();
		desc.BufferDesc.Height = clientRect.Height();
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.OutputWindow = m_hWnd;
		desc.SampleDesc.Count = 1;
		desc.Windowed = true;

		ComPtr<IDXGISwapChain> swapChain;
		DBG::ThrowIfFailed(m_dxgiFactory->CreateSwapChain(m_commandQueue.Get(), // Swap chain needs the queue so that it can force a flush on it.
			&desc, &swapChain));

		DBG::ThrowIfFailed(swapChain.As(&m_swapChain));
	}

	void Device::CreateRtvAndDsvDescriptorHeaps()
	{
		{
			D3D12_DESCRIPTOR_HEAP_DESC desc;
			desc.NumDescriptors = m_frameCount;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NodeMask = 0;
			DBG::ThrowIfFailed(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap)));
		}

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc;
			desc.NumDescriptors = 1;
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			desc.NodeMask = 0;
			DBG::ThrowIfFailed(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsvHeap)));
		}
	}
}