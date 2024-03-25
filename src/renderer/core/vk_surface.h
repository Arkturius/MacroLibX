/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vk_surface.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 18:57:55 by maldavid          #+#    #+#             */
/*   Updated: 2024/03/25 19:10:59 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_VK_SURFACE__
#define __MLX_VK_SURFACE__

namespace mlx
{
	class Surface
	{
		public:
			void create(class Renderer& renderer);
			void destroy() noexcept;
			
			VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

			inline VkSurfaceKHR& operator()() noexcept { return _surface; }
			inline VkSurfaceKHR& get() noexcept { return _surface; }

		private:
			VkSurfaceKHR _surface = VK_NULL_HANDLE;
	};
}

#endif // __MLX_VK_SURFACE__
