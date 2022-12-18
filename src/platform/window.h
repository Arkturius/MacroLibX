/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 21:53:12 by maldavid          #+#    #+#             */
/*   Updated: 2022/12/18 03:41:53 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MLX_WINDOW__
#define __MLX_WINDOW__

#include <SDL2/SDL.h>
#include <string>

namespace mlx
{
	class MLX_Window
	{
		public:
			MLX_Window(std::size_t w, std::size_t h, std::string title, int id);

			inline int& get_id() noexcept { return _id; }
			inline SDL_Window* getNativeWindow() const noexcept { return _win; }

			void pixel_put(int x, int y, int color) {}
			
			~MLX_Window();

		private:
			SDL_Window* _win = nullptr;
			int _id;
	};
}

#endif
