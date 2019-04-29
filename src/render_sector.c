/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:40:20 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 11:27:44 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void			get_wall_height(t_map *map, t_wall *wall,
											t_sector *sect, t_renderer *r)
{
	wall->ceil = sect->ceil_height - map->player.position.z;
	wall->floor = sect->floor_height - map->player.position.z;
	if (wall->neighbor >= 0)
	{
		wall->n_ceil = map->sectors[wall->neighbor].ceil_height -
													map->player.position.z;
		wall->n_floor = map->sectors[wall->neighbor].floor_height -
													map->player.position.z;
	}
	wall->y1.a = r->h / 2 - (int)((wall->ceil +
							wall->t1.y * map->player.pitch) * wall->scale1.y);
	wall->y1.b = r->h / 2 - (int)((wall->floor +
							wall->t1.y * map->player.pitch) * wall->scale1.y);
	wall->y2.a = r->h / 2 - (int)((wall->ceil +
							wall->t2.y * map->player.pitch) * wall->scale2.y);
	wall->y2.b = r->h / 2 - (int)((wall->floor +
							wall->t2.y * map->player.pitch) * wall->scale2.y);
	wall->n_y1.a = r->h / 2 - (int)((wall->n_ceil +
							wall->t1.y * map->player.pitch) * wall->scale1.y);
	wall->n_y1.b = r->h / 2 - (int)((wall->n_floor +
							wall->t1.y * map->player.pitch) * wall->scale1.y);
	wall->n_y2.a = r->h / 2 - (int)((wall->n_ceil +
							wall->t2.y * map->player.pitch) * wall->scale2.y);
	wall->n_y2.b = r->h / 2 - (int)((wall->n_floor +
							wall->t2.y * map->player.pitch) * wall->scale2.y);
}

static void			clamp_values(t_wall *wall)
{
	int		sign;

	sign = wall->t1.y < 0 ? -1 : 1;
	wall->t1.y = sign * maxf(fabs(wall->t1.y), 0.1f);
	sign = wall->t1.x < 0 ? -1 : 1;
	wall->t1.x = sign * maxf(fabs(wall->t1.x), 0.1f);
	sign = wall->t2.y < 0 ? -1 : 1;
	wall->t2.y = sign * maxf(fabs(wall->t2.y), 0.1f);
	sign = wall->t2.x < 0 ? -1 : 1;
	wall->t2.x = sign * maxf(fabs(wall->t2.x), 0.1f);
}

void				render_sector(t_main *m, t_renderer *r,
										t_render_item const *current_sector)
{
	size_t		s;
	t_sector	*sect;
	t_wall		wall;

	s = -1;
	sect = &m->map.sectors[current_sector->sectorno];
	ft_bzero(&wall, sizeof(wall));
	while (++s < sect->number_vertices)
	{
		setup_wall_texture(m, &wall, sect->textures[s], (t_pt){0, 1});
		wall.t1 = calculate_edges(&m->map.player, &sect->vertices[s]);
		wall.t2 = calculate_edges(&m->map.player, &sect->vertices[s + 1]);
		if (wall.t1.y < 0 && wall.t2.y < 0)
			continue;
		clamp_edges_with_player_view(&wall);
		clamp_values(&wall);
		do_perspective(&wall, m->sdl.img.w, m->sdl.img.h);
		if (wall.x1 >= wall.x2 || wall.x2 < current_sector->limit_x_left ||
									wall.x1 > current_sector->limit_x_right)
			continue;
		wall.neighbor = sect->neighbors[s];
		get_wall_height(&m->map, &wall, sect, r);
		render_wall(m, r, &wall, current_sector);
	}
}
