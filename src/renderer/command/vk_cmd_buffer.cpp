/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:26:06 by maldavid          #+#    #+#             */
/*   Updated: 2022/10/06 18:27:27 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_cmd_buffer.h"
#include <renderer/core/render_core.h>

namespace mlx
{
	void CmdBuffer::init()
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = Render_Core::get().getCmdPool().get();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if(vkAllocateCommandBuffers(Render_Core::get().getDevice().get(), &allocInfo, &_cmd_buffer) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate command buffer");
	}

	void CmdBuffer::destroy() noexcept
	{
		vkFreeCommandBuffers(Render_Core::get().getDevice().get(), Render_Core::get().getCmdPool().get(), 1, &_cmd_buffer);
	}

	void CmdBuffer::beginRecord(VkCommandBufferUsageFlags usage)
	{
		if(_is_recording)
			return;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage;
		if(vkBeginCommandBuffer(_cmd_buffer, &beginInfo) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to begin recording command buffer");

		_is_recording = true;
	}

	void CmdBuffer::endRecord()
	{
		if(!_is_recording)
			return;
		if(vkEndCommandBuffer(_cmd_buffer) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to end recording command buffer");

		_is_recording = false;
	}
}
