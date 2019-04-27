/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 15:37:10 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "map_parser.h"

static void			calc_delta_time(t_main *m)
{
	m->prev_time = m->curr_time;
	m->curr_time = SDL_GetPerformanceCounter();
	m->delta_time += (m->curr_time - m->prev_time) /
									(float)(SDL_GetPerformanceFrequency());
}

void				sdl_loop(t_main *m)
{
	m->map.player.is_moving = true;
	while (m->sdl.running)
	{
		calc_delta_time(m);
		sdl_hook(m);
		if (m->delta_time > 0.016f)
		{
			draw_screen(m);
			draw_sprites(m);
			draw_minimap(m);
			if (m->greenify)
				draw_green(m);
			sdl_put_image(&m->sdl);
			draw_gun(m);
			draw_hud(m);
			SDL_RenderPresent(m->sdl.ren);
			move_player(m);
			calc_sprite_collisions(m);
			m->delta_time = 0.f;
		}
		calc_green_time(m);
	}
}

static void			load_resources(t_main *m)
{
	load_textures(m);
	load_sprites(m);
	load_hud(m);
	load_sounds(m);
}

static void			unload_resources(t_main *m)
{
	unload_textures_and_sprites(m);
	unload_hud(m);
	unload_sounds(m);
}

int					main(int ac, char **av)
{
	t_main			m;

	ft_bzero(&m, sizeof(t_main));
	load_resources(&m);
	parser(&m.map, av[1]);
	m.sdl.win_w = W;
	m.sdl.win_h = H;
	sdl_init(&m.sdl);
	sdl_loop(&m);
	remove_data(&m.map);
	unload_resources(&m);
	SDL_Quit();
	return (0);
}
