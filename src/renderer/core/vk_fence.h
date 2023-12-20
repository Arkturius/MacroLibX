/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_fence.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 17:52:09 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/16 17:27:28 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_FENCE__
#define __MLX_VK_FENCE__

#include <volk.h>
#include <mlx_profile.h>

namespace mlx
{
	class Fence
	{
		public:
			Fence() = default;

			void init();

			inline VkFence& get() noexcept { return _fence; }
			void wait() noexcept;
			void reset() noexcept;
			bool isReady() const noexcept;
			inline void waitAndReset() noexcept { wait(); reset(); }

			void destroy() noexcept;

			~Fence() = default;

		private:
			VkFence _fence = VK_NULL_HANDLE;
	};
}

#endif
