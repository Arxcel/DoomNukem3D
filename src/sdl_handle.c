/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/28 18:01:54 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/29 14:18:45 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void			vertical_mivement_norm(t_main *m, int key, t_sector *s)
{
	if (key == SDLK_LCTRL || key == SDLK_RCTRL)
	{
		m->map.player.is_crouching = m->sdl.e.type == SDL_KEYDOWN;
		m->map.player.is_falling = true;
	}
	else if (key == SDLK_e)
	{
		s = &m->map.sectors[m->map.player.sector_number];
		if (m->map.player.is_standing && s->is_lift && !s->is_activated)
		{
			Mix_PlayChannel(-1, m->music.snd[4], 0);
			s->is_activated = true;
		}
	}
}

static void			vertical_movement(t_main *m, int key)
{
	t_sector *s;

	s = NULL;
	if (key == SDLK_r)
	{
		Mix_PlayChannel(-1, m->music.snd[1], 0);
		m->map.player.velocity.z += 0.2f;
		m->map.player.is_falling = true;
	}
	else if (key == ' ')
	{
		if (m->map.player.is_standing)
		{
			Mix_PlayChannel(-1, m->music.snd[3], 0);
			m->map.player.velocity.z += 0.5f;
			m->map.player.is_falling = true;
		}
	}
	vertical_mivement_norm(m, key, s);
}

static void			handle_key(t_main *m, int key)
{
	if (key == SDLK_q || key == SDLK_ESCAPE)
		m->sdl.running = 0;
	if (m->menu.is_active)
		handle_menu(m, key);
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
	else if (key == 9 && m->map.player.stats.total_active_weapon > 1)
	{
		if (m->map.player.stats.total_active_weapon == 2)
			m->map.player.stats.active_weapon = rand() % 2 + 1;
		if (m->map.player.stats.total_active_weapon == 3)
			m->map.player.stats.active_weapon = rand() % 3 + 1;
	}
	vertical_movement(m, key);
}

void				sdl_hook(t_main *m)
{
	while (SDL_PollEvent(&m->sdl.e))
	{
		if (m->sdl.e.type == SDL_QUIT)
			m->sdl.running = 0;
		else if (m->sdl.e.type == SDL_KEYDOWN || m->sdl.e.type == SDL_KEYUP)
			handle_key(m, m->sdl.e.key.keysym.sym);
		else if (m->sdl.e.type == SDL_MOUSEBUTTONUP)
		{
			if (m->map.player.stats.ammo > 0)
			{
				Mix_PlayChannel(-1, m->music.snd[8], 0);
				m->map.player.stats.ammo--;
			}
			m->hud.boom = 1;
		}
	}
}
