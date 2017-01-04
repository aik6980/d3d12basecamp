#include "stdafx.h"

#include "resource_mgr.h"

namespace VK
{
	void RESOURCE_MGR::create_uniform_buffer( uint32_t size )
	{
		vk::BufferCreateInfo create_info;
		create_info.usage       = vk::BufferUsageFlagBits::eUniformBuffer;
		create_info.size        = size;
		create_info.sharingMode = vk::SharingMode::eExclusive;

		auto buffer = m_device.createBuffer(create_info);

	}
}
