/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 18:01:54 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 11:52:46 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void			handle_key(t_main *m, int key)
{
	if (key == SDLK_ESCAPE || key == SDLK_q)
		m->sdl.running = 0;
	else if (key == SDLK_w)
		m->map.player.dir = m->sdl.e.type == SDL_KEYDOWN ? Forward : Nothing;
	else if (key == SDLK_s)
		m->map.player.dir = m->sdl.e.type == SDL_KEYDOWN ? Backward : Nothing;
	else if (key == SDLK_a)
		m->map.player.dir = m->sdl.e.type == SDL_KEYDOWN ? Left : Nothing;
	else if (key == SDLK_d)
		m->map.player.dir = m->sdl.e.type == SDL_KEYDOWN ? Right : Nothing;
	else if (key == ' ')
	{
		if (m->map.player.is_standing)
		{
			m->map.player.velocity.z += 0.5;
			m->map.player.is_falling = 1;
		}
	}
	else if (key == SDLK_LCTRL || key == SDLK_RCTRL)
	{
		m->map.player.is_crouching = m->sdl.e.type == SDL_KEYDOWN;
		m->map.player.is_falling = 1;
	}
}

void				sdl_hook(t_main *m)
{
	while (SDL_PollEvent(&m->sdl.e))
	{
		if (m->sdl.e.type == SDL_QUIT)
			m->sdl.running = 0;
		else if (m->sdl.e.type == SDL_WINDOWEVENT)
		{
			if (m->sdl.e.window.event == SDL_WINDOWEVENT_RESIZED)
				;
		}
		else if (m->sdl.e.type == SDL_KEYDOWN || m->sdl.e.type == SDL_KEYUP)
			handle_key(m, m->sdl.e.key.keysym.sym);
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
