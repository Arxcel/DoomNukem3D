/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 18:01:54 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 16:44:55 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void			vertical_movement(t_main *m, int key)
{
	if (key == SDLK_r)
	{
		m->map.player.velocity.z += 0.1f;
		m->map.player.is_falling = true;
	}
	else if (key == ' ')
	{
		
		if (m->map.player.is_standing)
		{
			Mix_PlayChannel(-1,m->music.snd[0], 0);
			m->map.player.velocity.z += 0.5f;
			m->map.player.is_falling = true;
		}
	}
	else if (key == SDLK_LCTRL || key == SDLK_RCTRL)
	{
		m->map.player.is_crouching = m->sdl.e.type == SDL_KEYDOWN;
		m->map.player.is_falling = true;
	}
}

static void			handle_key(t_main *m, int key)
{
	if (key == SDLK_q || key == SDLK_ESCAPE)
		m->sdl.running = 0;
	else if (key == SDLK_w)
		m->map.player.dir.forward = m->sdl.e.type == SDL_KEYDOWN;
	else if (key == SDLK_s)
		m->map.player.dir.backward = m->sdl.e.type == SDL_KEYDOWN;
	else if (key == SDLK_a)
		m->map.player.dir.left = m->sdl.e.type == SDL_KEYDOWN;
	else if (key == SDLK_d)
		m->map.player.dir.right = m->sdl.e.type == SDL_KEYDOWN;
	else if (key == SDLK_LSHIFT)
		m->map.player.is_running = m->sdl.e.type == SDL_KEYDOWN;
	vertical_movement(m, key);
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
