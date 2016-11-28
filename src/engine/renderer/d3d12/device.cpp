#include "stdafx.h"

#include "device.h"

#include "d3dx12.h"
#include "engine/core/debug/debugUtil.h"
#include "engine/core/debug/debugOutput.h"

namespace D3D12
{
	static const D3D_FEATURE_LEVEL REQUESTED_FEATURE_LEVEL = D3D_FEATURE_LEVEL_12_1;

	static const DXGI_FORMAT depthStencilFmt = DXGI_FORMAT_D24_UNORM_S8_UINT;

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
		FlushCommandQueue();

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
		DBG::ThrowIfFailed(m_swapChain->ResizeBuffers(m_frameCount, clientRect.Width(), clientRect.Height(), DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

		m_currBackBuffer = 0;

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
		for (int i = 0; i < m_frameCount; ++i)
		{
			DBG::ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_swapChainBuffer[i])));
			m_device->CreateRenderTargetView(m_swapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
			rtvHeapHandle.Offset(1, m_rtvDescHeapSize);
		}

		// resize Depth Buffer
		D3D12_RESOURCE_DESC depthDesc;
		depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		depthDesc.Alignment = 0;
		depthDesc.Width = clientRect.Width();
		depthDesc.Height = clientRect.Height();
		depthDesc.DepthOrArraySize = 1;
		depthDesc.MipLevels = 1;

		// the depth buffer.  because we need to create two views to the same resource:
		//   1. SRV format: DXGI_FORMAT_R24_UNORM_X8_TYPELESS
		//   2. DSV Format: DXGI_FORMAT_D24_UNORM_S8_UINT
		// we need to create the depth buffer resource with a typeless format.  
		depthDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

		depthDesc.SampleDesc.Quality = 0;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE clearVal;
		clearVal.Format = depthStencilFmt;
		clearVal.DepthStencil.Depth = 1.0f;
		clearVal.DepthStencil.Stencil = 0;

		DBG::ThrowIfFailed(m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)
			, D3D12_HEAP_FLAG_NONE
			, &depthDesc
			, D3D12_RESOURCE_STATE_COMMON
			, &clearVal
			, IID_PPV_ARGS(&m_depthBuffer)
		));

		// Create descriptor to mip level 0 of entire resource using the format of the resource.
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Format = depthStencilFmt;
		dsvDesc.Texture2D.MipSlice = 0;

		m_device->CreateDepthStencilView(m_depthBuffer.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

		// Transition the resource from its initial state to be used as a depth buffer.
		m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_depthBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

		// Execute the resize commands.
		DBG::ThrowIfFailed(m_commandList->Close());
		ID3D12CommandList* cmdList[] = { m_commandList.Get() };
		m_commandQueue->ExecuteCommandLists(1, cmdList);

		// Wait until resize is complete.
		FlushCommandQueue();

		// Update the viewport transform to cover the client area.
		m_screenViewport.TopLeftX = 0;
		m_screenViewport.TopLeftY = 0;
		m_screenViewport.Width = static_cast<float>(clientRect.Width());
		m_screenViewport.Height = static_cast<float>(clientRect.Height());
		m_screenViewport.MinDepth = 0.0f;
		m_screenViewport.MaxDepth = 1.0f;

		m_screenScissorRect = { 0, 0, clientRect.Width(), clientRect.Height() };
	}

	void Device::FlushCommandQueue()
	{
		// Advance the fence value to mark commands up to this fence point.
		m_currFence++;

		// Add an instruction to the command queue to set a new fence point.  Because we 
		// are on the GPU timeline, the new fence point won't be set until the GPU finishes
		// processing all the commands prior to this Signal().
		DBG::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_currFence));

		// Wait until the GPU has completed commands up to this fence point.
		if (m_fence->GetCompletedValue() < m_currFence)
		{
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);

			// Fire event when GPU hits current fence.
			DBG::ThrowIfFailed(m_fence->SetEventOnCompletion(m_currFence, eventHandle));

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
		::ZeroMemory(&desc, sizeof(D3D12_COMMAND_QUEUE_DESC));
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
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

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