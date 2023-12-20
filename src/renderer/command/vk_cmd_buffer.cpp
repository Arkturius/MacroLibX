/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_cmd_buffer.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/06 18:26:06 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/17 20:01:46 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vk_cmd_buffer.h"
#include <renderer/core/render_core.h>
#include <renderer/command/cmd_manager.h>
#include <renderer/core/vk_semaphore.h>
#include <renderer/buffers/vk_buffer.h>

namespace mlx
{
	void CmdBuffer::init(kind type, CmdManager* manager)
	{
		init(type, &manager->getCmdPool());
	}

	void CmdBuffer::init(kind type, CmdPool* pool)
	{
		_type = type;
		_pool = pool;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = pool->get();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if(vkAllocateCommandBuffers(Render_Core::get().getDevice().get(), &allocInfo, &_cmd_buffer) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to allocate command buffer");
		#ifdef DEBUG
			core::error::report(e_kind::message, "Vulkan : created new command buffer");
		#endif

		_fence.init();
		_state = state::idle;
	}

	void CmdBuffer::beginRecord(VkCommandBufferUsageFlags usage)
	{
		if(!isInit())
			core::error::report(e_kind::fatal_error, "Vulkan : begenning record on un uninit command buffer");
		if(_state == state::recording)
			return;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage;
		if(vkBeginCommandBuffer(_cmd_buffer, &beginInfo) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to begin recording command buffer");

		_state = state::recording;
	}

	void CmdBuffer::bindVertexBuffer(Buffer& buffer) const noexcept
	{
		if(!isRecording())
		{
			core::error::report(e_kind::warning, "Vulkan : trying to bind a vertex buffer to a non recording command buffer");
			return;
		}
		VkDeviceSize offset[] = { buffer.getOffset() };
		vkCmdBindVertexBuffers(_cmd_buffer, 0, 1, &buffer.get(), offset);
		buffer.recordedInCmdBuffer();
	}

	void CmdBuffer::endRecord()
	{
		if(!isInit())
			core::error::report(e_kind::fatal_error, "Vulkan : ending record on un uninit command buffer");
		if(_state != state::recording)
			return;
		if(vkEndCommandBuffer(_cmd_buffer) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan : failed to end recording command buffer");

		_state = state::idle;
	}

	void CmdBuffer::submitIdle() noexcept
	{
		auto device = Render_Core::get().getDevice().get();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_cmd_buffer;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		VkFence fence;
		vkCreateFence(device, &fenceCreateInfo, nullptr, &fence);
		vkResetFences(device, 1, &fence);
		vkQueueSubmit(Render_Core::get().getQueue().getGraphic(), 1, &submitInfo, fence);
		vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
		vkDestroyFence(device, fence, nullptr);
		_state = state::submitted;
		_state = state::ready;
	}

	void CmdBuffer::submit(Semaphore& semaphores) noexcept
	{
		VkSemaphore signalSemaphores[] = { semaphores.getRenderImageSemaphore() };
		VkSemaphore waitSemaphores[] = { semaphores.getImageSemaphore() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &_cmd_buffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if(vkQueueSubmit(Render_Core::get().getQueue().getGraphic(), 1, &submitInfo, _fence.get()) != VK_SUCCESS)
			core::error::report(e_kind::fatal_error, "Vulkan error : failed to submit draw command buffer");
		_state = state::submitted;
	}

	void CmdBuffer::destroy() noexcept
	{
		_fence.destroy();
		_cmd_buffer = VK_NULL_HANDLE;
		_state = state::uninit;
	}
}
