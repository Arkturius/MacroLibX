/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.inl                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maldavid <kbz_8.dev@akel-engine.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:13:55 by maldavid          #+#    #+#             */
/*   Updated: 2023/04/02 15:26:16 by maldavid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <core/graphics.h>

namespace mlx
{
	int& GraphicsSupport::getID() noexcept { return _id; }
	std::shared_ptr<MLX_Window> GraphicsSupport::getWindow() { return _window; }

	void GraphicsSupport::beginRender() noexcept
	{
		if(!_renderer->beginFrame())
			return;
		_proj = glm::ortho<float>(0, _window->getWidth(), 0, _window->getHeight());
		_renderer->getUniformBuffer()->setData(sizeof(_proj), &_proj);
	}

	void GraphicsSupport::clearRenderData() noexcept
	{
		_textures_to_render.clear();
		_pixel_put_pipeline.clear();
	}

	void GraphicsSupport::pixelPut(int x, int y, int color) noexcept
	{
		_pixel_put_pipeline.setPixel(x, y, color);
	}

	void GraphicsSupport::texturePut(std::shared_ptr<Texture> texture, int x, int y)
	{
		_textures_to_render.emplace(texture, x, y);
	}
}
