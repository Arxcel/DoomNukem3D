/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:40:20 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 15:33:20 by vkozlov          ###   ########.fr       */
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
		wall->neighbor_ceil = map->sectors[wall->neighbor].ceil_height -
													map->player.position.z;
		wall->neighbor_floor = map->sectors[wall->neighbor].floor_height -
													map->player.position.z;
	}
	wall->y1[0] = r->h / 2 - (int)((wall->ceil +
							r->t1.y * map->player.pitch) * wall->scale1.y);
	wall->y1[1] = r->h / 2 - (int)((wall->floor +
							r->t1.y * map->player.pitch) * wall->scale1.y);
	wall->y2[0] = r->h / 2 - (int)((wall->ceil +
							r->t2.y * map->player.pitch) * wall->scale2.y);
	wall->y2[1] = r->h / 2 - (int)((wall->floor +
							r->t2.y * map->player.pitch) * wall->scale2.y);
	wall->neighbor_y1[0] = r->h / 2 - (int)((wall->neighbor_ceil +
							r->t1.y * map->player.pitch) * wall->scale1.y);
	wall->neighbor_y1[1] = r->h / 2 - (int)((wall->neighbor_floor +
							r->t1.y * map->player.pitch) * wall->scale1.y);
	wall->neighbor_y2[0] = r->h / 2 - (int)((wall->neighbor_ceil +
							r->t2.y * map->player.pitch) * wall->scale2.y);
	wall->neighbor_y2[1] = r->h / 2 - (int)((wall->neighbor_floor +
							r->t2.y * map->player.pitch) * wall->scale2.y);
}

static void			clamp_values(t_renderer *r)
{
	int		sign;

	sign = r->t1.y < 0 ? -1 : 1;
	r->t1.y = sign * maxf(fabs(r->t1.y), 0.1f);
	sign = r->t1.x < 0 ? -1 : 1;
	r->t1.x = sign * maxf(fabs(r->t1.x), 0.1f);
	sign = r->t2.y < 0 ? -1 : 1;
	r->t2.y = sign * maxf(fabs(r->t2.y), 0.1f);
	sign = r->t2.x < 0 ? -1 : 1;
	r->t2.x = sign * maxf(fabs(r->t2.x), 0.1f);
}

static void			check_wall(t_renderer *r, t_map *map,
							int s, t_render_item const *current_sector)
{
	double		d;
	t_sector	*sect;

	sect = &map->sectors[current_sector->sectorno];
	d = cast_ray_2line(
		(t_vertex){map->player.position.x, map->player.position.y},
		(t_vertex){map->player.anglecos, map->player.anglesin},
		sect->vertices[s], sect->vertices[s + 1]);
	if (d > 0 && d < 3)
	{
		if (s == map->player.sector_number)
			return ;
		printf("facing sector %d\n", s);
		map->facing_sector = s;
		r->t1.x = maxf(fabs(r->t1.x), 0.1f);
	}
}

void				render_sector(t_main *m, t_renderer *r,
										t_render_item const *current_sector)
{
	int			s;
	t_sector	*sect;
	t_wall		wall;

	s = -1;
	sect = &m->map.sectors[current_sector->sectorno];
	ft_bzero(&wall, sizeof(wall));
	while (++s < sect->number_vertices)
	{
		setup_wall_texture(m, &wall, sect->textures[s], (t_pt){0, 1});
		r->t1 = calculate_edges(&m->map.player, &sect->vertices[s]);
		r->t2 = calculate_edges(&m->map.player, &sect->vertices[s + 1]);
		if (r->t1.y < 0 && r->t2.y < 0)
			continue;
		clamp_edges_with_player_view(r, &wall);
		clamp_values(r);
		check_wall(r, &m->map, s, current_sector);
		do_perspective(r, &wall, m->sdl.img.w, m->sdl.img.h);
		if (wall.x1 >= wall.x2 || wall.x2 < current_sector->limit_x_left ||
									wall.x1 > current_sector->limit_x_right)
			continue;
		wall.neighbor = sect->neighbors[s];
		get_wall_height(&m->map, &wall, sect, r);
		render_wall(m, r, &wall, current_sector);
	}
}
