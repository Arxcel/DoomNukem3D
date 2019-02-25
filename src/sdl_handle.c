/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 18:01:54 by vkozlov           #+#    #+#             */
/*   Updated: 2019/02/23 14:15:50 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomNukem.h"

static void			key_down(int key, t_main *m)
{
	static int a = 1;
	if (key == SDLK_ESCAPE)
		m->sdl.running = 0;
	else if (key == SDLK_w || key == SDLK_s || key == SDLK_a || key == SDLK_d
				|| key == SDLK_q || key == SDLK_e || key == SDLK_SPACE)
	{
		m->sdl.changes = 1;
		sdl_pixel_put(m->sdl.img, a, a, 255);
		a++;
	}
	else if (key == SDLK_i || key == SDLK_k || key == SDLK_j || key == SDLK_l
				|| key == SDLK_u || key == SDLK_o)
	{
		m->sdl.changes = 1;
	}
	else if (key == SDLK_z)
	{
	}
}

void				sdl_loop(t_main *m)
{
	while (m->sdl.running)
	{
		sdl_hook(m);
		if (m->sdl.changes)
		{
			if(m->sdl.changes)
			{
				SDL_UpdateTexture(m->sdl.texture, NULL, m->sdl.img.pixels, m->sdl.img.w * sizeof(unsigned int));
				SDL_RenderCopy(m->sdl.ren, m->sdl.texture, NULL, &m->ui.scene_place);
				SDL_RenderPresent(m->sdl.ren);
			}
			m->sdl.changes = 0;
		}
	}
}

void				sdl_hook(t_main *m)
{
	while (SDL_PollEvent(&m->sdl.e) != 0)
	{
		if (m->sdl.e.type == SDL_QUIT)
			m->sdl.running = 0;
		else if (m->sdl.e.type == SDL_WINDOWEVENT)
		{
			if (m->sdl.e.window.event == SDL_WINDOWEVENT_RESIZED)
				;
		}
		else if (m->sdl.e.type == SDL_KEYDOWN)
			key_down(m->sdl.e.key.keysym.sym, m);
		else if (m->sdl.e.type == SDL_MOUSEBUTTONDOWN)
			;
		else if (m->sdl.e.type == SDL_MOUSEBUTTONUP)
			;
		else if (m->sdl.e.type == SDL_MOUSEMOTION)
			;
		else if (m->sdl.e.type == SDL_MOUSEWHEEL)
			;
	}
}
