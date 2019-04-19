/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprites.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 14:01:22 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 14:06:34 by vkozlov          ###   ########.fr       */
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

static void		calc_sprite_edges(t_vector player_pos, t_vector sprite_pos,
										t_wall *wall, float dist)
{
	float v1_x_loc;
	float v1_y_loc;
	float v2_x_loc;
	float v2_y_loc;

	v1_x_loc = (sprite_pos.x - player_pos.x) / dist;
	v1_y_loc = (sprite_pos.y - player_pos.y) / dist;
	v2_x_loc = -v1_y_loc;
	v2_y_loc = v1_x_loc;
	wall->t1.x = (sprite_pos.x - player_pos.x);
	wall->t1.y = (sprite_pos.y - player_pos.y);
	wall->t2.x = (v2_x_loc * 1) + sprite_pos.x - player_pos.x;
	wall->t2.y = (v2_y_loc * 1) + sprite_pos.y - player_pos.y;
}

static void	calc_sprite_h(t_sprite *s, t_wall *wall, t_player *p, int h)
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

static void	draw_sprite(t_main *m, t_wall *wall, float dist)
{
	int	begin_x;
	int end_x;
	int		x;

	begin_x = wall->x1;
	end_x = begin_x + m->map.sprites[0].w * 100 / dist;
	if (end_x < 0)
		end_x = 0;
	x = begin_x;
	while (x < end_x)
	{
		if (x < W && x > 0)
		{
			if (x > begin_x - 1)
			{
				int y1;
				int y2;
				y1 = (370 + dist * 0.3) - m->map.player.pitch * 160;
				y2 = y1 + (end_x - begin_x);
				int		y;
				if (y2 > y1)
				{
					y = y1;
					while (y <= y2)
					{
						if (y >= y1)
							if (y < H && 0 < y)
								sdl_pixel_put(&m->sdl.img, x, y, 0xFF0000);
						y++;
					}
				}
			}
		}
		x++;
	}
}

void render_sprites(t_main *m)
{
	t_wall		wall;
	float		dist;
	int			i;

	i = -1;
	while (++i < m->map.number_sptites)
	{
		dist = calc_distance(m->map.player.position, m->map.sprites[i].position);
		calc_sprite_edges(m->map.player.position, m->map.sprites[i].position,
																	&wall, dist);
		wall.t1 = calculate_edges2(&m->map.player, &wall.t1);
		wall.t2 = calculate_edges2(&m->map.player, &wall.t2);
		if (wall.t1.y <= 0 && wall.t2.y <= 0)
			return ;
		do_perspective(&wall, m->sdl.img.w, m->sdl.img.h);
		calc_sprite_h(&m->map.sprites[i], &wall, &m->map.player, m->sdl.img.h);
		draw_sprite(m, &wall, dist);
	}
}
