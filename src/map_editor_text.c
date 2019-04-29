/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_text.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:49:06 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 19:17:11 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void			create_text_menu_impl(t_main *m,
									t_text *menu, SDL_Color bg)
{
	int i;

	i = -1;
	while (++i < TEXT_MENU)
	{
		menu[i].selected = false;
		menu[i].rows = (TEXT_MENU + 1) / 2;
		create_text(m, menu, i, bg);
	}
}

void				create_text_menu(t_main *m, t_text *menu)
{
	SDL_Color	bg;

	bg = (SDL_Color){255, 255, 255, 255};
	menu[0].text = ft_strdup("Sector:");
	menu[1].text = ft_strdup("Walls:");
	menu[2].text = ft_strdup("Texture:");
	menu[3].text = ft_strdup("Floor Height:");
	menu[4].text = ft_strdup("Ceiling Height:");
	menu[5].text = ft_strdup("Is lifted");
	menu[6].text = ft_strdup("From:");
	menu[7].text = ft_strdup("To:");
	menu[8].text = ft_strdup("0");
	menu[9].text = ft_strdup("0");
	menu[10].text = ft_strdup("0");
	menu[11].text = ft_strdup("0");
	menu[12].text = ft_strdup("100");
	menu[13].text = ft_strdup("0");
	menu[14].text = ft_strdup("0");
	menu[15].text = ft_strdup("0");
	create_text_menu_impl(m, menu, bg);
}

void				update_all_menu(t_main *m, t_map_editor *e)
{
	int i;

	i = -1;
	while (++i < TEXT_MENU)
		if (i != e->selected_row)
		{
			e->menu[i].selected = false;
			update_text_color(m, e->menu, i);
		}
	e->menu[e->selected_row].selected = true;
	update_text_color(m, e->menu, e->selected_row);
	update_text(m, e->menu, 8, e->n);
	if (e->sectors[e->n].num_walls > -1)
		update_text(m, e->menu, 9, e->sectors[e->n].num_walls);
	if (e->sectors[e->n].num_walls > 0)
		update_text(m, e->menu, 10,
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture);
	update_text(m, e->menu, 11, e->sectors[e->n].floor_height);
	update_text(m, e->menu, 12, e->sectors[e->n].ceiling_height);
	update_text(m, e->menu, 13, e->sectors[e->n].is_lift);
	update_text(m, e->menu, 14, e->sectors[e->n].from);
	update_text(m, e->menu, 15, e->sectors[e->n].to);
}

int					remove_text_menu(t_map_editor *e)
{
	int i;

	i = -1;
	while (++i < TEXT_MENU)
	{
		SDL_DestroyTexture(e->menu[i].text_texture);
		free(e->menu[i].text);
	}
	i = -1;
	while (++i < SPRITE_MENU)
	{
		SDL_DestroyTexture(e->sprite_menu[i].text_texture);
		free(e->sprite_menu[i].text);
	}
	return (0);
}
