/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 15:18:34 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/29 16:53:47 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void	sort(int num_sprites, int *order, float *dist)
{
	int			i;
	int			j;
	float		tempf;
	float		tempi;

	i = -1;
	while (++i < num_sprites)
	{
		j = -1;
		while (++j < num_sprites - i - 1)
		{
			if (dist[j] <= dist[j + 1])
			{
				tempf = dist[j + 1];
				dist[j + 1] = dist[j];
				dist[j] = tempf;
				tempi = order[j + 1];
				order[j + 1] = order[j];
				order[j] = tempi;
			}
		}
	}
}

void		setup_sprite_texture(t_main *m, t_wall *w, int wall)
{
	w->solid_id = wall % m->tex.s.num_textures;
}

void		sort_sprites(t_main *m, int *order, float *dist)
{
	size_t i;

	i = -1;
	while (++i < m->map.number_sprites)
	{
		dist[i] = calc_distance(m->map.player.position,
								m->map.sprites[i].position);
		order[i] = i;
	}
	sort(m->map.number_sprites, order, dist);
}

void		calc_sprites(t_player *p, t_vector sprite_pos,
										t_wall *wall, float dist)
{
	t_vertex loc1;
	t_vertex loc2;

	loc1.x = (sprite_pos.x - p->position.x) / dist;
	loc1.y = (sprite_pos.y - p->position.y) / dist;
	loc2.x = -loc1.y;
	loc2.y = loc1.x;
	wall->t1.x = sprite_pos.x - p->position.x;
	wall->t1.y = sprite_pos.y - p->position.y;
	wall->t2.x = loc2.x + sprite_pos.x - p->position.x;
	wall->t2.y = loc2.y + sprite_pos.y - p->position.y;
	wall->t1 = calculate_edges2(p, &wall->t1);
	wall->t2 = calculate_edges2(p, &wall->t2);
}

void		draw_sprite_line(t_main *m, t_wall *w, t_vline *v, t_interp *ty)
{
	int				y;
	int				*pix;
	SDL_Surface		*current;
	unsigned int	color;

	current = m->tex.s.textures[v->texture_id % m->tex.s.num_textures];
	pix = current->pixels;
	v->y_top = clampf(v->y_top, 0, m->sdl.img.h);
	v->y_bottom = clampf(v->y_bottom, 0, m->sdl.img.h);
	y = v->y_top - 1;
	while (++y < v->y_bottom)
	{
		w->txty = interp_next(ty);
		color = pix[w->txtx % current->w +
						(w->txty % current->h) * current->w];
		if (color)
		{
			color = c_darken(color, w->lz);
			sdl_pixel_put(&m->sdl.img, v->x, y, color);
		}
	}
}
