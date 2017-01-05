#include "stdafx.h"

#include "resource_mgr.h"
#include "device.h"

namespace VK
{
	void RESOURCE_MGR::create_uniform_buffer( uint32_t size )
	{
		auto&& device = m_device.get_device();

		vk::BufferCreateInfo create_info;
		create_info.usage       = vk::BufferUsageFlagBits::eUniformBuffer;
		create_info.size        = size;
		create_info.sharingMode = vk::SharingMode::eExclusive;

		auto buffer = device.createBuffer(create_info);
		auto buffer_mem_req	= device.getBufferMemoryRequirements(buffer);
		vk::MemoryAllocateInfo alloc_info;
		alloc_info.memoryTypeIndex = m_device.get_memory_type_from_properties(buffer_mem_req.memoryTypeBits, 
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

		auto data = device.allocateMemory(alloc_info);

		m_uniform_buffer.m_buffer = buffer;
		m_uniform_buffer.m_data = data;
	}

	void* RESOURCE_MGR::map(vk::DeviceMemory device_mem, uint32_t offset, uint32_t size)
	{
		auto&& device = m_device.get_device();
		return device.mapMemory(device_mem, offset, size);
	}

	void RESOURCE_MGR::unmap(vk::DeviceMemory device_mem)
	{
		auto&& device = m_device.get_device();
		return device.unmapMemory(device_mem);
	}

}
