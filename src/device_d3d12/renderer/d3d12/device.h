#pragma once

#include "d3dx12.h"
using Microsoft::WRL::ComPtr;

namespace D3D12
{
	class FRAME_RESOURCE;

	class DEVICE
	{
	public:
		friend class MESH_RENDERER;

		~DEVICE();

		void LoadPipeline(HWND hwnd);
		void OnResize();

		/// helper
		void begin_load_resources();
		void end_load_resources();
		void begin_frame();
		void end_frame();

		D3D12_VIEWPORT				get_window_viewport() const;
		CD3DX12_RECT				get_window_rect() const;

		/// low level
		ID3D12Device*				device()         { return m_device.Get(); }
		ID3D12GraphicsCommandList*	commmand_list()  { return m_commandList.Get(); }
		FRAME_RESOURCE&				frame_resource() { return *m_curr_frame_resource; }
		
		D3D12_CPU_DESCRIPTOR_HANDLE curr_backbuffer_view() const;
		D3D12_CPU_DESCRIPTOR_HANDLE curr_backbuffer_depth_stencil_view() const;
		ID3D12Resource*				curr_backbuffer() const;

		DXGI_SWAP_CHAIN_DESC		get_swap_chain_desc() const;
		DXGI_FORMAT					get_depth_stencil_format() const { return m_depth_stencil_format; }
	private:
		void FindHardwareAdapter(IDXGIFactory4& factory);

		void CreateCommandObjects();
		void CreateSwapChain();

		void CreateRtvAndDsvDescriptorHeaps();

		void flush_command_queue();
		void reset_current_command_allocator();

		// frame resource function
		void build_frame_resource_list();

		static const uint32_t		m_frameCount = 2;
		static const DXGI_FORMAT	m_depth_stencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		/// win32
		HWND						m_hWnd;

		ComPtr<IDXGIFactory4>		m_dxgiFactory;
		/// pipeline state
		ComPtr<IDXGIAdapter1>		m_adapter;
		ComPtr<ID3D12Device>		m_device;
		
		ComPtr<ID3D12CommandQueue>			m_commandQueue;
		/// using the frame resource instead
		//ComPtr<ID3D12CommandAllocator>	m_commandAlloc;
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


		//Frame Resources
		const static UINT m_num_frame_resources = 3;
		vector<unique_ptr<FRAME_RESOURCE>>	m_frame_resource_list;
		FRAME_RESOURCE*						m_curr_frame_resource = nullptr;
		UINT								m_curr_frame_resource_index = 0;
	};
}