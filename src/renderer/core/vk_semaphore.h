/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_semaphore.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:59:38 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 17:55:10 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_SEMAPHORE__
#define __MLX_VK_SEMAPHORE__

#include <volk.h>
#include <vector>

namespace mlx
{
	class Semaphore
	{
		public:
			void init();
			void destroy() noexcept;

			inline VkSemaphore& getImageSemaphore() noexcept { return _imageAvailableSemaphores; }
			inline VkSemaphore& getRenderImageSemaphore() noexcept { return _renderFinishedSemaphores; }

		private:
			VkSemaphore _imageAvailableSemaphores = VK_NULL_HANDLE;
			VkSemaphore _renderFinishedSemaphores = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_SEMAPHORE__
