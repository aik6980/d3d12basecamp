#pragma once

namespace VK 
{
	class RESOURCE_MGR
	{
	public:
		void init(vk::Device device)
		{
			m_device = device;
		}

		void destroy();

		void create_uniform_buffer( uint32_t size );
	private:
		vk::Device	m_device;
	};
}