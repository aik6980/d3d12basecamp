#pragma once

#include "stdafx.h"

namespace VK
{
	inline void throw_if_failed(VkResult hr)
	{
		if (hr != VK_SUCCESS) throw;
	}

	// struct
	struct GPU
	{
		vk::PhysicalDevice                 m_device;
		vk::PhysicalDeviceMemoryProperties m_memory_properties;
		vk::PhysicalDeviceProperties       m_properties;
	};

	struct TEXTURE
	{
		vk::Format			  m_format;
		vk::Image			  m_image;
		vk::DeviceMemory	  m_data;
		vector<vk::ImageView> m_view_list;
	};
}