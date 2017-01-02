#include "stdafx.h"

#include "device.h"

#include "common/common.cpp.h"
#include "global.h"

namespace VK
{

	void DEVICE::init(const INIT_DATA& init_info)
	{
		m_init_data = init_info;
		init_create_instance();
		init_enumerate_physical_devices();
		init_win32_surface();
		init_device();
		init_command_pool();
		init_command_buffer();
		init_swapchain();
		init_depth_buffer();
	}

	void DEVICE::destroy()
	{
		destroy_depth_buffer();
		destroy_swapchain();
		destroy_command_buffer();
		destroy_command_pool();
		destroy_win32_surface();
		destroy_device();
		destroy_instance();
	}

	void DEVICE::init_create_instance()
	{
		static const char* APP_SHORT_NAME = "vulkan_basecamp";

		// initialize the VkApplicationInfo structure
		vk::ApplicationInfo app_info;
		app_info.pApplicationName = APP_SHORT_NAME;
		app_info.applicationVersion = 1;
		app_info.pEngineName = APP_SHORT_NAME;
		app_info.engineVersion = 1;
		app_info.apiVersion = VK_API_VERSION_1_0;

		// extensions declaration
		array<const char*, 2> extension_names = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };

		// initialize the VkInstanceCreateInfo structure
		vk::InstanceCreateInfo inst_info;
		inst_info.pApplicationInfo        = &app_info;
		inst_info.enabledExtensionCount   = (uint32_t)extension_names.size();
		inst_info.ppEnabledExtensionNames = extension_names.data();

		vk::Result hresult;
		hresult = vk::createInstance(&inst_info, NULL, &m_instance);
		if (hresult == vk::Result::eErrorIncompatibleDriver)
		{
			DBG::OutputString("cannot find a compatible Vulkan ICD\n");
		}
		else if (hresult != vk::Result::eSuccess) 
		{
			DBG::OutputString("unknown error\n");
		}
	}

	void DEVICE::destroy_instance()
	{
		m_instance.destroy(nullptr);
	}

	vk::Extent2D DEVICE::get_client_rect()
	{
		CRect client_rect;
		GetClientRect(m_init_data.hwnd, &client_rect);
		vk::Extent2D extent;
		extent.width = client_rect.Width();
		extent.height = client_rect.Height();

		return extent;
	}

	uint32_t DEVICE::get_memory_type_from_properties(const vk::PhysicalDeviceMemoryProperties memory_properties, uint32_t typebits, vk::MemoryPropertyFlags requirements_mask)
	{
		// Search mem types to find first index with those properties
		for (auto i=0u; i < memory_properties.memoryTypeCount; ++i)
		{
			if (typebits & 1)
			{
				// Type is available, does it match user properties?
				if ((memory_properties.memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask)
				{
					return i;
				}
			}
			typebits >>= 1;
		}

		// No memory types matched, return failure
		throw;
		return UINT32_MAX;
	}

	void DEVICE::init_enumerate_physical_devices()
	{
		// enumerate through available devices
		uint32_t gpu_count = 0;

		auto physical_device_list = m_instance.enumeratePhysicalDevices();

		for (auto&& physical_device : physical_device_list)
		{
			GPU device;
			device.m_device            = physical_device;
			device.m_properties        = physical_device.getProperties();
			device.m_memory_properties = physical_device.getMemoryProperties();
			m_device_list.emplace_back(device);
		}
	}

	void DEVICE::init_device()
	{
		auto&& selected_device = m_device_list[0].m_device;

		vector<vk::QueueFamilyProperties> queue_family_list = selected_device.getQueueFamilyProperties();
		assert(queue_family_list.size() > 0);

		// make sure this device is supporting at least QUEUE_GRAPHIC
		// one queue family can support more than one type of queue
		uint32_t graphic_queue_idx = UINT32_MAX;
		uint32_t compute_queue_idx = UINT32_MAX;
		for (auto i=0; i<queue_family_list.size(); ++i)
		{
			auto&& queue_family = queue_family_list[i];
			
			if (queue_family.queueFlags & vk::QueueFlagBits::eGraphics &&
				selected_device.getSurfaceSupportKHR(i, m_surface))
			{
				graphic_queue_idx = i;
				m_prim_graphic_queue_family_idx = i;
			}
			if (queue_family.queueFlags & vk::QueueFlagBits::eCompute)
			{
				compute_queue_idx = i;
			}
		}

		vk::DeviceQueueCreateInfo queue_info;
		queue_info.pNext = nullptr;
		queue_info.queueCount = 1;
		float queue_priority[] = { 0.0f };
		queue_info.pQueuePriorities = queue_priority;

		// extensions declaration
		array<const char*, 1> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		vk::DeviceCreateInfo device_info    = {};
		device_info.queueCreateInfoCount    = 1;
		device_info.pQueueCreateInfos       = &queue_info;
		device_info.enabledExtensionCount   = (uint32_t)extensions.size();
		device_info.ppEnabledExtensionNames = extensions.data();
		device_info.enabledLayerCount       = 0;
		device_info.ppEnabledLayerNames     = nullptr;
		device_info.pEnabledFeatures        = nullptr;

		m_device = selected_device.createDevice(device_info);
	}

	void DEVICE::init_command_pool()
	{
		vk::CommandPoolCreateInfo info;
		info.queueFamilyIndex = m_prim_graphic_queue_family_idx;

		m_prim_command_pool = m_device.createCommandPool(info);
	}

	void DEVICE::init_command_buffer()
	{
		vk::CommandBufferAllocateInfo info;
		info.commandPool = m_prim_command_pool;
		info.level = vk::CommandBufferLevel::ePrimary;
		info.commandBufferCount = 1;

		vector<vk::CommandBuffer> command_buffer_list;
		command_buffer_list = m_device.allocateCommandBuffers(info);
		assert(command_buffer_list.size() > 0);
		m_prim_command_buffer = command_buffer_list[0];
	}

	void DEVICE::init_win32_surface()
	{
		auto&& selected_device = m_device_list[0].m_device;

		vk::Win32SurfaceCreateInfoKHR info;
		info.setHinstance(m_init_data.instance);
		info.setHwnd(m_init_data.hwnd);

		m_surface = m_instance.createWin32SurfaceKHR(info);

		// Get the list of VkFormats that are supported:
		vector<vk::SurfaceFormatKHR> surface_fmt_list;
		surface_fmt_list = selected_device.getSurfaceFormatsKHR(m_surface);

		// If the format list includes just one entry of VK_FORMAT_UNDEFINED,
		// the surface has no preferred format.  Otherwise, at least one
		// supported format will be returned.
		if (surface_fmt_list.size() == 1 && surface_fmt_list[0].format == vk::Format::eUndefined)
		{
			m_surface_seleted_fmt = vk::Format::eB8G8R8A8Unorm;
		}
		else
		{
			assert(surface_fmt_list.size() > 1);
			m_surface_seleted_fmt = surface_fmt_list[0].format;
		}
	}

	void DEVICE::init_swapchain()
	{
		auto&& selected_device = m_device_list[0].m_device;

		auto surface_caps = selected_device.getSurfaceCapabilitiesKHR(m_surface);

		vk::Extent2D extent = surface_caps.currentExtent;
		// width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
		if (surface_caps.currentExtent.width == UINT32_MAX)
		{
			// If the surface size is undefined, the size is set to
			// the size of the images requested.
			extent = get_client_rect();

			// todo: clamp width and height to min/max from surface caps
		}

		// The FIFO present mode is guaranteed by the spec to be supported
		vk::PresentModeKHR swapchain_present_mode = vk::PresentModeKHR::eFifo;

		vk::SurfaceTransformFlagBitsKHR pretransform = surface_caps.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity ?
			vk::SurfaceTransformFlagBitsKHR::eIdentity : surface_caps.currentTransform;

		vk::SwapchainCreateInfoKHR create_info;
		create_info.surface = m_surface;
		// Determine the number of VkImage's to use in the swap chain.
		// We need to acquire only 1 presentable image at at time.
		// Asking for minImageCount images ensures that we can acquire
		// 1 presentable image as long as we present it before attempting
		// to acquire another.
		create_info.minImageCount = surface_caps.minImageCount;
		create_info.imageFormat = m_surface_seleted_fmt;
		create_info.imageExtent = extent;
		create_info.preTransform = pretransform;
		create_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		create_info.imageArrayLayers = 1;
		create_info.presentMode = swapchain_present_mode;
		create_info.clipped = true;
		create_info.imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
		create_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
		create_info.imageSharingMode = vk::SharingMode::eExclusive;

		// If the graphics and present queues are from different queue families,
		// we either have to explicitly transfer ownership of images between
		// the queues, or we have to create the swapchain with imageSharingMode
		// as VK_SHARING_MODE_CONCURRENT
		// Todo : Only if we need this?

		m_swapchain = m_device.createSwapchainKHR(create_info);
		m_swapchain_buffer_list = m_device.getSwapchainImagesKHR(m_swapchain);

		for (auto&& buffer : m_swapchain_buffer_list)
		{
			vk::ImageViewCreateInfo create_info;
			create_info.image = buffer;
			create_info.format = m_surface_seleted_fmt;
			create_info.viewType = vk::ImageViewType::e2D;
			// https://www.khronos.org/registry/vulkan/specs/1.0/man/html/VkComponentMapping.html
			create_info.subresourceRange = vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor,
				0, 1, 0, 1);

			auto image_view = m_device.createImageView(create_info);
			m_buffer_image_view_list.emplace_back(image_view);
		}
	}

	void DEVICE::init_depth_buffer()
	{
		auto&& selected_gpu = m_device_list[0];
		auto&& selected_device = selected_gpu.m_device;

		vk::Format selected_fmt = vk::Format::eD24UnormS8Uint;
		auto props = selected_device.getFormatProperties(selected_fmt);

		vk::ImageTiling tiling_mode;
		if (props.linearTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
		{
			tiling_mode = vk::ImageTiling::eLinear;
		}
		else if (props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
		{
			tiling_mode = vk::ImageTiling::eOptimal;
		}
		else
		{
			// selected_fmt is not supported;
			throw;
		}

		auto extent = get_client_rect();

		vk::ImageCreateInfo ci;
		ci.imageType   = vk::ImageType::e2D;
		ci.format      = selected_fmt;
		ci.extent      = vk::Extent3D(extent.width, extent.height, 1);
		ci.mipLevels   = 1;
		ci.arrayLayers = 1;
		ci.samples     = vk::SampleCountFlagBits::e1;
		ci.usage       = vk::ImageUsageFlagBits::eDepthStencilAttachment;

		vk::MemoryAllocateInfo mem_alloc;

		vk::ImageViewCreateInfo view_info;
		view_info.format = selected_fmt;
		view_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
		view_info.subresourceRange.layerCount = 1;
		view_info.subresourceRange.levelCount = 1;
		view_info.viewType = vk::ImageViewType::e2D;

		m_depth_buffer.m_format = selected_fmt;
		m_depth_buffer.m_image = m_device.createImage(ci);
		auto mem_reqs = m_device.getImageMemoryRequirements(m_depth_buffer.m_image);
		
		mem_alloc.allocationSize = mem_reqs.size;
		mem_alloc.memoryTypeIndex = get_memory_type_from_properties(selected_gpu.m_memory_properties, 
			mem_reqs.memoryTypeBits, vk::MemoryPropertyFlags());

		m_depth_buffer.m_data = m_device.allocateMemory(mem_alloc);
		m_device.bindImageMemory(m_depth_buffer.m_image, m_depth_buffer.m_data, 0);

		view_info.image = m_depth_buffer.m_image;
		auto depth_view = m_device.createImageView(view_info);
		m_depth_buffer.m_view_list.emplace_back(depth_view);
	}

	void DEVICE::destroy_depth_buffer()
	{
		for (auto&& view : m_depth_buffer.m_view_list)
		{
			m_device.destroyImageView(view);
		}

		m_device.destroyImage(m_depth_buffer.m_image);
		m_device.freeMemory(m_depth_buffer.m_data);
	}

	void DEVICE::destroy_swapchain()
	{
		for (auto&& img_view : m_buffer_image_view_list)
		{
			m_device.destroyImageView(img_view);
		}
		m_buffer_image_view_list.clear();
		m_swapchain_buffer_list.clear();
		m_device.destroySwapchainKHR(m_swapchain);
	}

	void DEVICE::destroy_win32_surface()
	{
		m_instance.destroySurfaceKHR(m_surface);
		DestroyWindow(m_init_data.hwnd);
	}

	void DEVICE::destroy_command_buffer()
	{
		array<vk::CommandBuffer, 1> cmd_bufs = { m_prim_command_buffer };
		m_device.freeCommandBuffers(m_prim_command_pool, cmd_bufs);
	}

	void DEVICE::destroy_command_pool()
	{
		m_device.destroyCommandPool(m_prim_command_pool);
	}

	void DEVICE::destroy_device()
	{
		m_device.waitIdle();
		m_device.destroy();
	}

}