/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:40:20 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/31 14:27:47 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static t_vertex		calculate_edges(t_player *player, t_vertex *vertex)
{
	t_vertex	res;

	res.x = (vertex->x - player->position.x) * player->anglesin -
						(vertex->y - player->position.y) * player->anglecos;
	res.y = (vertex->x - player->position.x) * player->anglecos +
						(vertex->y - player->position.y) * player->anglesin;
	return (res);
}

static void			clamp_edges_with_player_view(t_renderer *r,
																	t_wall *w)
{
	t_vertex	i1;
	t_vertex	i2;
	t_vertex	org1;
	t_vertex	org2;

	i1 = intersect_line(r->t1, r->t2, (t_vertex){-NEAR_SIDE, NEAR_Z},
						(t_vertex){-FAR_SIDE, FAR_Z});
	i2 = intersect_line(r->t1, r->t2, (t_vertex){NEAR_SIDE, NEAR_Z},
						(t_vertex){FAR_SIDE, FAR_Z});
	org1 = r->t1;
	org2 = r->t2;
	if (r->t1.y < NEAR_Z)
		clamp_point(&r->t1, &i1, &i2);
	if (r->t2.y < NEAR_Z)
		clamp_point(&r->t2, &i1, &i2);
	if (fabsf(r->t2.x - r->t1.x) > fabsf(r->t2.y - r->t1.y))
	{
		w->u0 = (r->t1.x - org1.x) * 1023.0 / (org2.x - org1.x);
		w->u1 = (r->t2.x - org1.x) * 1023.0 / (org2.x - org1.x);
	}
	else
	{
		w->u0 = (r->t1.y - org1.y) * 1023.0 / (org2.y - org1.y);
		w->u1 = (r->t2.y - org1.y) * 1023.0 / (org2.y - org1.y);
	}
}

static void			do_perspective(t_renderer *renderer, t_wall *w,
													int width, int height)
{
	w->scale1.x = HFOV / renderer->t1.y * width;
	w->scale1.y = VFOV / renderer->t1.y * height;
	w->x1 = width / 2 - (int)(renderer->t1.x * w->scale1.x);
	w->scale2.x = HFOV / renderer->t2.y * width;
	w->scale2.y = VFOV / renderer->t2.y * height;
	w->x2 = width / 2 - (int)(renderer->t2.x * w->scale2.x);
}

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
		r->t1.x = maxf(fabs(r->t1.x), 0.1f);
}

static void			setup_wall_texture(t_main *m, t_wall *w, int s)
{
	w->u0 = 0;
	w->u1 = 1023;
	w->floor_id = 0 % m->tex.t.numTextures;
	w->ceil_id = 1 % m->tex.t.numTextures;
	w->upper_id = s % m->tex.t.numTextures;
	w->lower_id = s % m->tex.t.numTextures;
	w->solid_id = s % m->tex.t.numTextures;
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
		setup_wall_texture(m, &wall, s);
		r->t1 = calculate_edges(&m->map.player, &sect->vertices[s]);
		r->t2 = calculate_edges(&m->map.player, &sect->vertices[s + 1]);
		if (r->t1.y < 0 && r->t2.y < 0)
			continue;
		if (r->t1.y <= 0 || r->t2.y <= 0)
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
