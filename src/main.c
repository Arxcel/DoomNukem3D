/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 11:41:44 by vkozlov          ###   ########.fr       */
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
			sdl_put_image(&m->sdl);
			draw_gun(m);
			draw_hud(m);
			SDL_RenderPresent(m->sdl.ren);
			move_player(m);
			calc_sprite_collisions(m);
			m->delta_time = 0.f;
		}
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

static void			init_sprite(t_main *m)
{
	int i;

	i = -1;
	m->map.number_sprites = m->tex.s.num_textures;
	m->map.sprites = (t_sprite*)malloc(sizeof(t_sprite) *
													m->map.number_sprites);
	while (++i < m->map.number_sprites)
	{
		m->map.sprites[i].position = (t_vector){15 + i, 15, 15.0};
		m->map.sprites[i].w = 10;
		m->map.sprites[i].h = 5;
		m->map.sprites[i].is_active = true;
		m->map.sprites[i].texture = i;
	}
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
	init_sprite(&m);
	sdl_loop(&m);
	remove_data(&m.map);
	unload_resources(&m);
	SDL_Quit();
	return (0);
}
