/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_option.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 17:20:32 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/28 18:22:30 by vkozlov          ###   ########.fr       */
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
		init_map_editor(m);
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

static void	select_level(t_main *m, char *name)
{
	parser(&m->map, name);
	m->menu.is_level_select = false;
	m->menu.is_active = false;
	Mix_PlayChannel(-1, m->music.snd[9], 0);
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

void		check_menu_active_option(t_main *m, int check)
{
	if (check == 0)
	{
		if (m->menu.active_option > 4)
			m->menu.active_option = 0;
		else if (m->menu.active_option < 0)
			m->menu.active_option = 4;
	}
	if (check == 1)
	{
		if (m->menu.active_option > 2)
			m->menu.active_option = 0;
		else if (m->menu.active_option < 0)
			m->menu.active_option = 2;
	}
}
