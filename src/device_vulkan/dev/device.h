#pragma once

#include "stdafx.h"
#include "device_vulkan.h"

namespace VK
{
	class DEVICE
	{
	public:
		struct INIT_DATA
		{
			HINSTANCE	instance;
			HWND		hwnd;
		};

		void init(const INIT_DATA& init_info);
		void destroy();

		void init_create_instance();
		void init_enumerate_physical_devices();
		void init_device();
		void init_command_pool();
		void init_command_buffer();
		void init_win32_surface();
		void init_swapchain();
		void init_depth_buffer();

		void destroy_depth_buffer();
		void destroy_swapchain();
		void destroy_win32_surface();
		void destroy_command_buffer();
		void destroy_command_pool();
		void destroy_device();
		void destroy_instance();

		vk::Extent2D get_client_rect();
		uint32_t get_memory_type_from_properties(const vk::PhysicalDeviceMemoryProperties memory_properties, 
			uint32_t typebits, vk::MemoryPropertyFlags requirements_mask);
	private:
		INIT_DATA				m_init_data;

		vk::Instance			m_instance;
		vector<GPU>	            m_device_list;

		uint32_t				m_prim_graphic_queue_family_idx = -1;
		vk::CommandPool			m_prim_command_pool;
		vk::CommandBuffer		m_prim_command_buffer;

		vk::Device				m_device;

		vk::SurfaceKHR			m_surface;
		vk::Format				m_surface_seleted_fmt;

		vk::SwapchainKHR		m_swapchain;
		vector<vk::Image>		m_swapchain_buffer_list;
		vector<vk::ImageView>	m_buffer_image_view_list;

		TEXTURE					m_depth_buffer;
	};
}