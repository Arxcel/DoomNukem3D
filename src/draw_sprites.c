/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arxcel <arxcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 14:01:22 by vkozlov           #+#    #+#             */
/*   Updated: 2019/05/14 00:16:26 by arxcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static bool			need_to_render(t_main *m, t_wall *w, float d, int i)
{
	return (!m->map.sprites[i].is_active || (w->t1.y <= 0 && w->t2.y <= 0)
			|| d < 2 || d > 15);
}

static void			calc_sprite_h(t_sprite *s, t_wall *wall,
													t_player *p, int h)
{
	wall->ceil = s->position.z + s->h - p->position.z;
	wall->floor = s->position.z - p->position.z;
	wall->y1.a = h / 2 - (int)((wall->ceil +
							wall->t1.y * p->pitch) * wall->scale1.y);
	wall->y1.b = h / 2 - (int)((wall->floor +
							wall->t1.y * p->pitch) * wall->scale1.y);
	wall->y2.a = h / 2 - (int)((wall->ceil +
							wall->t2.y * p->pitch) * wall->scale2.y);
	wall->y2.b = h / 2 - (int)((wall->floor +
							wall->t2.y * p->pitch) * wall->scale2.y);
}

static void			draw_sprite_impl_y(t_main *m, t_wall *wall,
													int x)
{
	SDL_Surface		*current;
	t_interp		*i;

	current = m->tex.s.textures[wall->solid_id];
	wall->ya = (x - wall->x1) * (wall->y2.a - wall->y1.a) /
										(wall->x2 - wall->x1) + wall->y1.a;
	wall->yb = (x - wall->x1) * (wall->y2.b - wall->y1.b) /
										(wall->x2 - wall->x1) + wall->y1.b;
	wall->cya = clampf(wall->ya, 0, m->sdl.win_h - 1);
	wall->cyb = clampf(wall->yb, 0, m->sdl.win_h - 1);
	i = init_interp((t_pt){wall->ya, wall->cya},
								wall->yb, (t_pt){0, current->h});
	draw_sprite_line(m, wall, &(t_vline){x, wall->cya,
									wall->cyb, wall->solid_id}, i);
	free(i);
}

static void			draw_sprite(t_main *m, t_wall *wall, float dist, int s)
{
	size_t			begin_x;
	int				end_x;
	size_t			x;
	t_interp		*ix;

	begin_x = wall->x1;
	end_x = begin_x + m->map.sprites[s].w * SPRITE_SCALE_X / dist;
	if (end_x < 0)
		end_x = 0;
	x = begin_x - 1;
	ix = init_interp((t_pt){begin_x, begin_x},
					end_x, (t_pt){0, m->tex.s.textures[wall->solid_id]->w});
	while (++x < (size_t)end_x)
	{
		wall->txtx = interp_next(ix);
		wall->lz = dist * m->map.player.darkness * 4;
		if (x < (size_t)m->sdl.win_w && x > 0)
			if (x > begin_x - 1)
				draw_sprite_impl_y(m, wall, x);
	}
	free(ix);
}

void				draw_sprites(t_main *m)
{
	t_wall		w;
	int			i;
	int			*o;
	float		*d;
	size_t		j;

	o = (int*)malloc(sizeof(int) * m->map.number_sprites);
	d = (float*)malloc(sizeof(float) * m->map.number_sprites);
	sort_sprites(m, o, d);
	j = -1;
	while (++j < m->map.number_sprites)
	{
		i = o[j];
		d[0] = calc_distance(m->map.player.position,
								m->map.sprites[i].position);
		setup_sprite_texture(m, &w, m->map.sprites[i].texture);
		calc_sprites(&m->map.player, m->map.sprites[i].position, &w, d[0]);
		if (need_to_render(m, &w, d[0], i))
			continue ;
		do_perspective(&w, m->sdl.win_w, m->sdl.win_h);
		calc_sprite_h(&m->map.sprites[i], &w, &m->map.player, m->sdl.win_h);
		draw_sprite(m, &w, d[0], i);
	}
	free(o);
	free(d);
}
