/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_modes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 15:44:17 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 18:01:44 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void		standard_mode(t_main *m)
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
	calc_green_time(m);
}

void		menu_mode(t_main *m)
{
	draw_menu(m);
	m->delta_time = 0.f;
}

void		victory_mode(t_main *m)
{
	draw_victory(m);
	m->delta_time = 0.f;
}
