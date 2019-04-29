/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_sprites.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 19:06:28 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 16:08:43 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void    create_sprite_menu(t_main *m, t_text *sprite_menu)
{
    int i;
	SDL_Color bg = {255, 255, 255, 255};

    sprite_menu[0].text = ft_strdup("Sprite z");
	sprite_menu[1].text = ft_strdup("Sprite texture:");
	sprite_menu[2].text = ft_strdup("Darkness:");
	sprite_menu[3].text = ft_strdup("Gravity:");
    sprite_menu[4].text = ft_strdup("0");
	sprite_menu[5].text = ft_strdup("0");
	sprite_menu[6].text =  ft_strdup("2");
	sprite_menu[7].text = ft_strdup("1");
    i = -1;
	while( ++i < SPRITE_MENU)
	{
		sprite_menu[i].selected = false;
        sprite_menu[i].rows = SPRITE_MENU / 2;
		create_text(m, sprite_menu, i, bg);
	}
}

void				update_sprite_menu(t_main *m, t_map_editor *e)
{
	int i;

	i = -1;
	while (++i < SPRITE_MENU)
		if (i != e->selected_row)
		{
			e->sprite_menu[i].selected = false;
			update_text_color(m, e->sprite_menu, i);
		}
	e->sprite_menu[e->selected_row].selected = true;
	update_text_color(m, e->sprite_menu, e->selected_row);
    update_text(m, e->sprite_menu, 4, (int)e->sprites[e->sprite_cnt].position.z);
    update_text(m, e->sprite_menu, 5, e->sprites[e->sprite_cnt].texture);
    update_text(m, e->sprite_menu, 6, m->map.player.darkness);
    update_text(m, e->sprite_menu, 7, (int)m->map.player.gravity);
}