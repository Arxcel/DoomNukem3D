/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/28 18:33:04 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void	calc_delta_time(t_main *m)
{
	m->prev_time = m->curr_time;
	m->curr_time = SDL_GetPerformanceCounter();
	m->delta_time += (m->curr_time - m->prev_time) /
									(float)(SDL_GetPerformanceFrequency());
}

void		sdl_loop(t_main *m)
{
	m->map.player.is_moving = true;
	while (m->sdl.running)
	{
		calc_delta_time(m);
		sdl_hook(m);
		if (m->delta_time > 0.016f && !m->victory && !m->menu.is_active)
			standard_mode(m);
		else if (m->delta_time > 0.016f && m->victory && !m->menu.is_active)
			victory_mode(m);
		else if (m->delta_time > 0.016f && !m->victory && m->menu.is_active)
			menu_mode(m);
	}
}

void		load_resources(t_main *m)
{
	load_textures(m);
	load_sprites(m);
	load_hud(m);
	load_sounds(m);
}

void		unload_resources(t_main *m)
{
	remove_data(&m->map);
	unload_textures_and_sprites(m);
	unload_hud(m);
	unload_sounds(m);
}

// int			main(int ac, char **av)
int			main()
{
	t_main	m;

	ft_bzero(&m, sizeof(t_main));
	load_resources(&m);
	m.menu.is_active = true;
	m.sdl.win_w = W;
	m.sdl.win_h = H;
	sdl_init(&m.sdl);
	sdl_loop(&m);
	unload_resources(&m);
	editor_clear_sdl(&m.sdl);
	return (0);
}
