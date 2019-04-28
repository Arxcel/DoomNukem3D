/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 17:20:32 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/28 12:37:16 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void		do_general_opt(t_main *m)
{
	if (m->menu.active_option == 0)
	{
		m->menu.is_level_select = true;
		m->menu.active_option = 0;
	}
	else if (m->menu.active_option == 1)
	{
		if (init_map_editor(m))
		{
			printf("failed init\n");
			unload_resources(m);
			editor_clear_sdl(&m->sdl);
			exit(0);
		}
		printf("loop start\n");
		map_editor_loop(m);
		printf("loop finish\n");
		unload_resources(m);
		editor_clear_sdl(&m->sdl);
		exit(0);
	}
	else if (m->menu.active_option == 2)
		m->sdl.running = false;
}

static void	init_sprite(t_main *m)
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

static void	select_level(t_main *m, char *name)
{
	parser(&m->map, name);
	init_sprite(m);
	m->menu.is_level_select = false;
	m->menu.is_active = false;
}

void		do_select_opt(t_main *m)
{
	if (m->menu.active_option == 0)
		select_level(m, "map1");
	else if (m->menu.active_option == 1)
		select_level(m, "map2");
	else if (m->menu.active_option == 2)
		select_level(m, "map3");
	else if (m->menu.active_option == 3)
		select_level(m, "map4");
	else if (m->menu.active_option == 4)
	{
		m->menu.is_level_select = false;
		m->menu.active_option = 0;
	}
}
