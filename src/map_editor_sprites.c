/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_sprites.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 19:06:28 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 21:00:16 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void				create_sprite_menu(t_main *m, t_text *sprite_menu)
{
	int			i;
	SDL_Color	bg;

	bg = (SDL_Color){255, 255, 255, 255};
	sprite_menu[0].text = ft_strdup("Sprite z");
	sprite_menu[1].text = ft_strdup("Sprite texture:");
	sprite_menu[2].text = ft_strdup("Darkness:");
	sprite_menu[3].text = ft_strdup("Gravity:");
	sprite_menu[4].text = ft_strdup("0");
	sprite_menu[5].text = ft_strdup("0");
	sprite_menu[6].text = ft_strdup("2");
	sprite_menu[7].text = ft_strdup("3");
	i = -1;
	while (++i < SPRITE_MENU)
	{
		sprite_menu[i].selected = false;
		sprite_menu[i].rows = SPRITE_MENU / 2;
		create_text(m, sprite_menu, i, bg);
	}
}

static void			update_sprite_menu_impl(t_main *m, t_map_editor *e)
{
	update_text_color(m, e->sprite_menu, e->selected_row);
	if (e->sprite_cnt)
		update_text(m, e->sprite_menu, 4,
						(int)e->sprites[e->sprite_cnt - 1].position.z);
	if (e->sprite_cnt)
		update_text(m, e->sprite_menu, 5,
								e->sprites[e->sprite_cnt - 1].texture);
	update_text(m, e->sprite_menu, 6, m->map.player.darkness);
	update_text(m, e->sprite_menu, 7, (int)m->map.player.gravity);
}

void				update_sprite_menu(t_main *m, t_map_editor *e)
{
	int i;

	i = -1;
	if (e->mode == SPRITE_Z)
		e->selected_row = 0;
	else if (e->mode == SPRITE_TEXTURE)
		e->selected_row = 1;
	else if (e->mode == DARKNESS)
		e->selected_row = 2;
	else if (e->mode == GRAVITY)
		e->selected_row = 3;
	while (++i < SPRITE_MENU)
		if (i != e->selected_row)
		{
			e->sprite_menu[i].selected = false;
			update_text_color(m, e->sprite_menu, i);
		}
	e->sprite_menu[e->selected_row].selected = true;
	update_sprite_menu_impl(m, e);
}

void				draw_sprites_dot(t_map_editor *e, t_main *m)
{
	int i;

	if (m->sdl.e.type == SDL_KEYDOWN
		|| m->sdl.e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (e->mode < SPRITE_Z)
			update_all_menu(m, e);
		else
			update_sprite_menu(m, e);
	}
	e->chosen = draw(m, e);
	if (e->mode >= PLAYER)
		draw_circle(RED, m, m->map.player.position);
	i = -1;
	if (e->mode >= SPRITE_Z && e->sprite_cnt)
		while (++i < e->sprite_cnt)
			draw_circle(YELLOW, m, e->sprites[i].position);
}
