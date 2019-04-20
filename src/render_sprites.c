/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprites.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 14:01:22 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 16:41:14 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static float		calc_distance(t_vector v1, t_vector v2)
{
	float	abx;
	float	aby;
	float	dist;

	abx = (v1.x - v2.x);
	aby = (v1.y - v2.y);
	dist = (float)sqrt(abx * abx + aby * aby);
	return (dist);
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
	wall->cya = clampf(wall->ya, 0, H - 1);
	wall->cyb = clampf(wall->yb, 0, H - 1);
	i = init_interp((t_pt){wall->ya, wall->cya},
								wall->yb, (t_pt){0, current->h});
	draw_sprite_line(m, wall, &(t_vline){x, wall->cya,
									wall->cyb, wall->solid_id}, i);
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
		wall->lz = ((x - wall->x1) * (wall->t2.y - wall->t1.y) /
							(wall->x2 - wall->x1) + wall->t1.y) * DARKNESS;
		if (x < m->sdl.img.w && x > 0)
			if (x > begin_x - 1)
				draw_sprite_impl_y(m, wall, x, dist);
	}
	free(ix);
}

void				render_sprites(t_main *m)
{
	t_wall		wall;
	float		d;
	int			i;

	i = -1;
	while (++i < m->map.number_sptites)
	{
		setup_sprite_texture(m, &wall, m->map.sprites[i].texture);
		d = calc_distance(m->map.player.position, m->map.sprites[i].position);
		if (d < 1)
			continue;
		calc_sprite_edges(m->map.player.position, m->map.sprites[i].position,
																	&wall, d);
		wall.t1 = calculate_edges2(&m->map.player, &wall.t1);
		wall.t2 = calculate_edges2(&m->map.player, &wall.t2);
		if (wall.t1.y <= 0 && wall.t2.y <= 0)
			return ;
		do_perspective(&wall, m->sdl.img.w, m->sdl.img.h);
		calc_sprite_h(&m->map.sprites[i], &wall, &m->map.player, m->sdl.img.h);
		draw_sprite(m, &wall, d, i);
	}
}
