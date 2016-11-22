#pragma once

using Microsoft::WRL::ComPtr;

namespace D3D12
{
	class Device
	{
	public:
		void LoadPipeline(HWND hwnd);
		void OnResize();
	private:
		void FindHardwareAdapter(IDXGIFactory4& factory);

		void CreateCommandObjects();
		void CreateSwapChain();

		void CreateRtvAndDsvDescriptorHeaps();

		void FlushCommmandQueue();

		static const uint32_t		m_frameCount = 2;

		/// win32
		HWND						m_hWnd;

		ComPtr<IDXGIFactory4>		m_dxgiFactory;
		/// pipeline state
		ComPtr<IDXGIAdapter1>		m_adapter;
		ComPtr<ID3D12Device>		m_device;
		
		ComPtr<ID3D12CommandQueue>			m_commandQueue;
		ComPtr<ID3D12CommandAllocator>		m_commandAlloc;
		ComPtr<ID3D12GraphicsCommandList>	m_commandList;

		ComPtr<IDXGISwapChain3>		m_swapChain;
		UINT						m_currBackBuffer = 0;

		uint64_t					m_currFence = 0;
		ComPtr<ID3D12Fence>			m_fence;

		UINT	m_rtvDescHeapSize = 0;
		UINT	m_dsvDescHeapSize = 0;
		UINT	m_cbvSrvUavDescHeapSize = 0;
		ComPtr<ID3D12DescriptorHeap>	m_rtvHeap;
		ComPtr<ID3D12DescriptorHeap>	m_dsvHeap;

		ComPtr<ID3D12Resource> m_swapChainBuffer[m_frameCount];
		ComPtr<ID3D12Resource> m_depthBuffer;
	};
}