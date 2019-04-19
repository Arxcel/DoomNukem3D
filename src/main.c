/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 14:18:05 by vkozlov          ###   ########.fr       */
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
			m->delta_time = 0.f;
		}
	}
}

static void initSprite(t_main *m)
{
	m->map.sprites = (t_sprite*)malloc(sizeof(t_sprite) * 1);
	m->map.number_sptites = 1;
	// m->map.sprites[0].position = (t_vector){75.0,174.0,0.0};
	m->map.sprites[0].position = (t_vector){15.0,15.0,0.0};
	m->map.sprites[0].w = 30;
	m->map.sprites[0].h = 20;
	m->map.sprites[0].is_active = true;
	m->map.sprites[0].texture = 0;
	m->map.sprites[0].sprite_ceil = 30;
	m->map.sprites[0].sprite_floor = 0;

	// m->map.player.angle = 2;
	// m->map.player.position = (t_vector){75.0,147.0,0.0};

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
	initSprite(&m);
	sdl_loop(&m);
	clear_textures(&m);
	remove_data(&m.map);
	Mix_CloseAudio();
	SDL_Quit();
	return (0);
}
