/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 17:36:44 by maldavid          #+#    #+#             */
/*   Updated: 2023/12/09 16:52:29 by kbz_8            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <platform/window.h>
#include <core/errors.h>
#include <utils/icon_mlx.h>
#include <iostream>

namespace mlx
{
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		constexpr const uint32_t rmask = 0xff000000;
		constexpr const uint32_t gmask = 0x00ff0000;
		constexpr const uint32_t bmask = 0x0000ff00;
		constexpr const uint32_t amask = 0x000000ff;
	#else
		constexpr const uint32_t rmask = 0x000000ff;
		constexpr const uint32_t gmask = 0x0000ff00;
		constexpr const uint32_t bmask = 0x00ff0000;
		constexpr const uint32_t amask = 0xff000000;
	#endif

	MLX_Window::MLX_Window(std::size_t w, std::size_t h, const std::string& title) : _width(w), _height(h)
	{
		_win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN);
		if(!_win)
			core::error::report(e_kind::fatal_error, std::string("unable to open a new window, ") + SDL_GetError());
		_id = SDL_GetWindowID(_win);
		_icon = SDL_CreateRGBSurfaceFrom(static_cast<void*>(logo_mlx), logo_mlx_width, logo_mlx_height, 32, 4 * logo_mlx_width, rmask, gmask, bmask, amask);
		SDL_SetWindowIcon(_win, _icon);
	}

	void MLX_Window::destroy() noexcept
	{
		std::cout << "prout" << std::endl;
		if(_win != nullptr)
		{
			SDL_DestroyWindow(_win);
			_win = nullptr;
		}
		if(_icon != nullptr)
		{
			SDL_FreeSurface(_icon);
			_icon = nullptr;
		}
	}
}
