/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:31:51 by kbz_8             #+#    #+#             */
/*   Updated: 2024/03/25 19:13:05 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_MEMORY__
#define __MLX_MEMORY__

#include <utils/singleton.h>

namespace mlx
{
	class MemManager : public Singleton<MemManager>
	{
		friend class Singleton<MemManager>;

		public:
			static void* malloc(std::size_t size);
			static void* calloc(std::size_t n, std::size_t size);
			static void* realloc(void* ptr, std::size_t size);
			static void free(void* ptr);

		private:
			MemManager() = default;
			~MemManager();

		private:
			inline static std::list<void*> _blocks;
	};
}

#endif
