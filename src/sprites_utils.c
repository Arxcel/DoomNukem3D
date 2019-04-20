/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 15:18:34 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 16:40:38 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void		setup_sprite_texture(t_main *m, t_wall *w, int wall)
{
	w->solid_id = wall % m->tex.t.num_textures;
}

void		calc_sprite_edges(t_vector player_pos, t_vector sprite_pos,
										t_wall *wall, float dist)
{
	t_vertex loc1;
	t_vertex loc2;

	loc1.x = (sprite_pos.x - player_pos.x) / dist;
	loc1.y = (sprite_pos.y - player_pos.y) / dist;
	loc2.x = -loc1.y;
	loc2.y = loc1.x;
	wall->t1.x = sprite_pos.x - player_pos.x;
	wall->t1.y = sprite_pos.y - player_pos.y;
	wall->t2.x = loc2.x + sprite_pos.x - player_pos.x;
	wall->t2.y = loc2.y + sprite_pos.y - player_pos.y;
}

void		draw_sprite_line(t_main *m, t_wall *w, t_vline *v, t_interp *ty)
{
	int				y;
	int				*pix;
	SDL_Surface		*current;
	unsigned int	color;

	current = m->tex.s.textures[v->texture_id % m->tex.s.num_textures];
	pix = current->pixels;
	v->y_top = clampf(v->y_top, 0, m->sdl.img.h - 1);
	v->y_bottom = clampf(v->y_bottom, 0, m->sdl.img.h - 1);
	y = v->y_top;
	while (++y <= v->y_bottom)
	{
		w->txty = interp_next(ty);
		color = c_darken(pix[w->txtx % current->w +
						(w->txty % current->h) * current->w], w->lz);
		if (color)
			sdl_pixel_put(&m->sdl.img, v->x, y, color);
	}
}
