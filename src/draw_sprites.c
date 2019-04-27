/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 14:01:22 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 11:14:43 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static bool			need_to_render(t_main *m, t_wall *w, float d, int i)
{
	return (!m->map.sprites[i].is_active || (w->t1.y <= 0 && w->t2.y <= 0)
			|| (d < 1 || d > 15 ||
			m->map.sprites[i].position.z - m->map.player.position.z > 2));
}

static void			calc_sprite_h(t_sprite *s, t_wall *wall,
													t_player *p, int h)
{
	wall->ceil = s->position.z + s->h - p->position.z;
	wall->floor = s->position.z - p->position.z;
	wall->y1[0] = h / 2 - (int)((wall->ceil +
							wall->t1.y * p->pitch) * wall->scale1.y);
	wall->y1[1] = h / 2 - (int)((wall->floor +
							wall->t1.y * p->pitch) * wall->scale1.y);
	wall->y2[0] = h / 2 - (int)((wall->ceil +
							wall->t2.y * p->pitch) * wall->scale2.y);
	wall->y2[1] = h / 2 - (int)((wall->floor +
							wall->t2.y * p->pitch) * wall->scale2.y);
}

static void			draw_sprite_impl_y(t_main *m, t_wall *wall,
													int x, float dist)
{
	int				y1;
	int				y2;
	int				y;
	SDL_Surface		*current;
	t_interp		*i;

	current = m->tex.s.textures[wall->solid_id];
	wall->ya = (x - wall->x1) * (wall->y2[0] - wall->y1[0]) /
										(wall->x2 - wall->x1) + wall->y1[0];
	wall->yb = (x - wall->x1) * (wall->y2[1] - wall->y1[1]) /
										(wall->x2 - wall->x1) + wall->y1[1];
	wall->cya = clampf(wall->ya, 0, m->sdl.img.h - 1);
	wall->cyb = clampf(wall->yb, 0, m->sdl.img.h - 1);
	i = init_interp((t_pt){wall->ya, wall->cya},
								wall->yb, (t_pt){0, current->h});
	draw_sprite_line(m, wall, &(t_vline){x, wall->cya,
									wall->cyb, wall->solid_id}, i);
	free(i);
}

static void			draw_sprite(t_main *m, t_wall *wall, float dist, int s)
{
	int				begin_x;
	int				end_x;
	int				x;
	t_interp		*ix;

	begin_x = wall->x1;
	end_x = begin_x + m->map.sprites[s].w * SPRITE_SCALE_X / dist;
	if (end_x < 0)
		end_x = 0;
	x = begin_x - 1;
	ix = init_interp((t_pt){begin_x, begin_x},
					end_x, (t_pt){0, m->tex.s.textures[wall->solid_id]->w});
	while (++x < end_x)
	{
		wall->txtx = interp_next(ix);
		wall->lz = dist * DARKNESS * 4;
		if (x < m->sdl.img.w && x > 0)
			if (x > begin_x - 1)
				draw_sprite_impl_y(m, wall, x, dist);
	}
	free(ix);
}

void				draw_sprites(t_main *m)
{
	t_wall		wall;
	int			i;
	int			*s_order;
	float		*s_dist;
	int			j;

	s_order = (int*)malloc(sizeof(int) * m->map.number_sprites);
	s_dist = (float*)malloc(sizeof(float) * m->map.number_sprites);
	sort_sprites(m, s_order, s_dist);
	j = -1;
	while (++j < m->map.number_sprites)
	{
		i = s_order[j];
		setup_sprite_texture(m, &wall, m->map.sprites[i].texture);
		calc_sprite_edges(&m->map.player, m->map.sprites[i].position,
													&wall, s_dist[s_order[i]]);
		if (need_to_render(m, &wall, s_dist[s_order[i]], i))
			continue ;
		do_perspective(&wall, m->sdl.img.w, m->sdl.img.h);
		calc_sprite_h(&m->map.sprites[i], &wall, &m->map.player, m->sdl.img.h);
		draw_sprite(m, &wall, s_dist[s_order[i]], i);
	}
	free(s_order);
	free(s_dist);
}
