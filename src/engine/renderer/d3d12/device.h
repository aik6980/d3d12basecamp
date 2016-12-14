#pragma once

using Microsoft::WRL::ComPtr;

namespace D3D12
{
	class DEVICE
	{
	public:
		friend class MESH_RENDERER;

		void LoadPipeline(HWND hwnd);
		void OnResize();

		/// helper
		void begin_load_resources();
		void end_load_resources();
		void begin_frame();
		void end_frame();

		/// low level
		ID3D12GraphicsCommandList*	commmand_list() { return m_commandList.Get(); }
		D3D12_CPU_DESCRIPTOR_HANDLE curr_backbuffer_view() const;
	private:
		void FindHardwareAdapter(IDXGIFactory4& factory);

		void CreateCommandObjects();
		void CreateSwapChain();

		void CreateRtvAndDsvDescriptorHeaps();

		void flush_command_queue();

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

		D3D12_VIEWPORT	m_screenViewport;
		D3D12_RECT		m_screenScissorRect;
	};
}