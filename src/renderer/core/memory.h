/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 02:13:03 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 17:49:47 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_MEMORY__
#define __MLX_VK_MEMORY__

#include <pre_compiled.h>

namespace mlx
{
	class GPUallocator
	{
		public:
			GPUallocator() = default;

			void init() noexcept;
			void destroy() noexcept;

			VmaAllocation createBuffer(const VkBufferCreateInfo* binfo, const VmaAllocationCreateInfo* vinfo, VkBuffer& buffer, const char* name = nullptr) noexcept;
			void destroyBuffer(VmaAllocation allocation, VkBuffer buffer) noexcept;

			VmaAllocation createImage(const VkImageCreateInfo* iminfo, const VmaAllocationCreateInfo* vinfo, VkImage& image, const char* name = nullptr) noexcept;
			void destroyImage(VmaAllocation allocation, VkImage image) noexcept;

			void mapMemory(VmaAllocation allocation, void** data) noexcept;
			void unmapMemory(VmaAllocation allocation) noexcept;

			void dumpMemoryToJson();

			void flush(VmaAllocation allocation, VkDeviceSize size, VkDeviceSize offset) noexcept;

			~GPUallocator() = default;

		private:
			VmaAllocator _allocator;
			std::int32_t _active_buffers_allocations = 0;
			std::int32_t _active_images_allocations = 0;
	};
}

#endif
