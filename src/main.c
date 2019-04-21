/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/21 12:20:29 by vkozlov          ###   ########.fr       */
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
	while (m->sdl.running)
	{
		calc_delta_time(m);
		sdl_hook(m);
		if (m->delta_time > 0.016f)
		{
			draw_screen(m);
			draw_minimap(m);
			render_sprites(m);
			sdl_put_image(&m->sdl);
			move_player(m);
			calk_sprite_collisions(m);
			m->delta_time = 0.f;
		}
	}
}

static void			init_sprite(t_main *m)
{
	int i;

	i = -1;
	m->map.number_sprites = 5;
	m->map.sprites = (t_sprite*)malloc(sizeof(t_sprite) *
													m->map.number_sprites);
	while (++i < m->map.number_sprites)
	{
		m->map.sprites[i].position = (t_vector){15.0, 15.0 + 1 * i, 15.0};
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
	load_textures_snd(&m);
	parser(&m.map, av[1]);
	m.sdl.win_w = W;
	m.sdl.win_h = H;
	sdl_init(&m.sdl);
	init_sprite(&m);
	sdl_loop(&m);
	clear_textures(&m);
	remove_data(&m.map);
	Mix_CloseAudio();
	SDL_Quit();
	return (0);
}
