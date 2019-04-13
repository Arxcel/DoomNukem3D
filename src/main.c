/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 13:39:00 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "map_parser.h"
#include "map_editor.h"

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
			sdl_put_image(&m->sdl);
			move_player(m);
			m->delta_time = 0.f;
		}
	}
}

int					main(int ac, char **av)
{
	t_main			m;

	(void)av;
	ft_bzero(&m, sizeof(t_main));
	m.sdl.win_w = W;
	m.sdl.win_h = H;
	sdl_init(&m.sdl);
	if (ac > 1 && !ft_strcmp("-edit_map", av[1]))
	{
		init_map_editor(&m);
		map_editor_loop(&m);
	}
	else
	{
		load_textures(&m);
		parser(&m.map, av[1]);
		sdl_loop(&m);
		clear_textures(&m);
		remove_data(&m.map);
	}
	SDL_Quit();
	return (0);
}
