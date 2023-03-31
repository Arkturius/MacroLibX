/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_core.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 23:33:34 by maldavid          #+#    #+#             */
/*   Updated: 2023/03/31 12:22:34 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define VOLK_IMPLEMENTATION

#if defined(_WIN32) || defined(_WIN64)
	#define VK_USE_PLATFORM_WIN32_KHR
#elif defined(__APPLE__) || defined(__MACH__)
	#define VK_USE_PLATFORM_MACOS_MVK
#else
	#define VK_USE_PLATFORM_XLIB_KHR
#endif

#include "render_core.h"
#include <mutex>

namespace mlx
{
	std::mutex mutex;

	namespace RCore
	{
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
		{
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(Render_Core::get().getDevice().getPhysicalDevice(), &memProperties);

			for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			{
				if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
			}

			core::error::report(e_kind::fatal_error, "Vulkan : failed to find suitable memory type");
			return -1; // just to avoid warning
		}
	}

	void Render_Core::init()
	{
		volkInitialize();

		_instance.init();
		volkLoadInstance(_instance.get());
		_layers.init();
		_device.init();
		volkLoadDevice(_device.get());
		_queues.init();
		_is_init = true;
	}

	void Render_Core::destroy()
	{
        std::unique_lock<std::mutex> watchdog(mutex, std::try_to_lock);

		if(!_is_init)
			return;

        vkDeviceWaitIdle(_device());

		_device.destroy();
		_layers.destroy();
		_instance.destroy();

		_is_init = false;
	}
}
