#pragma once

#include "device_vulkan.h"

namespace VK 
{
	class DEVICE;

	class RESOURCE_MGR
	{
	public:
		RESOURCE_MGR(VK::DEVICE& device)
			: m_device(device)
		{ }

		void destroy();

		void create_uniform_buffer( uint32_t size );

		void*	map(vk::DeviceMemory device_mem, uint32_t offset, uint32_t size);
		void	unmap(vk::DeviceMemory device_mem);
	private:
		DEVICE&	m_device;

		// this should be a list
		UNIFORM_BUFFER	m_uniform_buffer;
	};
}