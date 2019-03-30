/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:40:20 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 13:11:46 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static double ray_segment_intersaction(t_vertex v1, t_vertex v2, t_vertex v3)
{
	double dot;
	double t1;
	double t2;

	dot = v_dot(v2, v3);
	if (fabs(dot) < 0.000001)
		return (-1.0);
	t1 = crossf(v2.x, v2.y, v1.x, v1.y) / dot;
	t2 = v_dot(v1, v3) / dot;
	if (t1 >= 0.0 && (t2 >= 0.0 && t2 <= 1.0))
		return (t1);
	return (-1.0);
}

static double cast_ray_2line(t_vertex ray_origin, t_vertex ray_direction, t_vertex point1, t_vertex point2)
{
	t_vertex v1;
	t_vertex v2;
	t_vertex v3;

	v1.x = ray_origin.x - point1.x;
	v1.y = ray_origin.y - point1.y;
	v2.x = point2.x - point1.x;
	v2.y = point2.y - point1.y;
	v3 = (t_vertex){-ray_direction.y, ray_direction.x};
	return (ray_segment_intersaction(v1, v2, v3));
}

static t_vector		calculate_edges(t_player *player, t_vertex *vertex)
{
	t_vector	res;

	res.x = (vertex->x - player->position.x) * player->anglesin -
						(vertex->y - player->position.y) * player->anglecos;
	res.z = (vertex->x - player->position.x) * player->anglecos +
						(vertex->y - player->position.y) * player->anglesin;

	return (res);
}

static void			clamp_edges_with_player_view(t_renderer *renderer, t_wall *w)
{
	t_vertex	i1;
	t_vertex	i2;
	t_vertex	org1;
	t_vertex	org2;


	i1 = intersect_line((t_vertex){renderer->t1.x, renderer->t1.z},
						(t_vertex){renderer->t2.x, renderer->t2.z},
						(t_vertex){-NEAR_SIDE, NEAR_Z},
						(t_vertex){-FAR_SIDE, FAR_Z});
	i2 = intersect_line((t_vertex){renderer->t1.x, renderer->t1.z},
						(t_vertex){renderer->t2.x, renderer->t2.z},
						(t_vertex){NEAR_SIDE, NEAR_Z},
						(t_vertex){FAR_SIDE, FAR_Z});
	org1 = (t_vertex){renderer->t1.x, renderer->t1.z};
	org2 = (t_vertex){renderer->t2.x, renderer->t2.z};
	if (renderer->t1.z < NEAR_Z)
		clamp_point(&renderer->t1, &i1, &i2);
	if (renderer->t2.z < NEAR_Z)
		clamp_point(&renderer->t2, &i1, &i2);
	if(fabsf(renderer->t2.x - renderer->t1.x) > fabsf(renderer->t2.z - renderer->t1.z))
	{
		w->u0 = (renderer->t1.x - org1.x) * 1023.0 / (org2.x - org1.x);
		w->u1 = (renderer->t2.x - org1.x) * 1023.0 / (org2.x - org1.x);
	}
	else
	{
		w->u0 = (renderer->t1.z - org1.y) * 1023.0 / (org2.y - org1.y);
		w->u1 = (renderer->t2.z - org1.y) * 1023.0 / (org2.y - org1.y);
	}
}

static void		do_perspective(t_renderer *renderer, t_wall *w, int width, int height)
{
	w->scale1.x = HFOV / renderer->t1.z * width;
	w->scale1.y = VFOV / renderer->t1.z * height;
	w->x1 = width / 2 - (int)(renderer->t1.x * w->scale1.x);
	w->scale2.x = HFOV / renderer->t2.z * width;
	w->scale2.y = VFOV / renderer->t2.z * height;
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
							r->t1.z * map->player.pitch) * wall->scale1.y);
	wall->y1[1] = r->h / 2 - (int)((wall->floor +
							r->t1.z * map->player.pitch) * wall->scale1.y);
	wall->y2[0] = r->h / 2 - (int)((wall->ceil +
							r->t2.z * map->player.pitch) * wall->scale2.y);
	wall->y2[1] = r->h / 2 - (int)((wall->floor +
							r->t2.z * map->player.pitch) * wall->scale2.y);
	wall->neighbor_y1[0] = r->h / 2 - (int)((wall->neighbor_ceil +
							r->t1.z * map->player.pitch) * wall->scale1.y);
	wall->neighbor_y1[1] = r->h / 2 - (int)((wall->neighbor_floor +
							r->t1.z * map->player.pitch) * wall->scale1.y);
	wall->neighbor_y2[0] = r->h / 2 - (int)((wall->neighbor_ceil +
							r->t2.z * map->player.pitch) * wall->scale2.y);
	wall->neighbor_y2[1] = r->h / 2 - (int)((wall->neighbor_floor +
							r->t2.z * map->player.pitch) * wall->scale2.y);
}


static void clamp_values(t_renderer *r)
{
	int		sign;

	sign = r->t1.z < 0 ? -1 : 1;
	r->t1.z  = sign * maxf(fabs(r->t1.z ), 0.1f);
	sign = r->t1.x < 0 ? -1 : 1;
	r->t1.x  = sign * maxf(fabs(r->t1.x), 0.1f);
	sign = r->t2.z < 0 ? -1 : 1;
	r->t2.z  = sign * maxf(fabs(r->t2.z ), 0.1f);
	sign = r->t2.x < 0 ? -1 : 1;
	r->t2.x  = sign * maxf(fabs(r->t2.x), 0.1f);
}

static void check_wall(t_renderer *r, t_map *map, int s, t_render_item const *current_sector)
{
	double d;
	t_sector	*sect;

	sect = &map->sectors[current_sector->sectorno];
	d = cast_ray_2line(
		(t_vertex){map->player.position.x, map->player.position.y},
		(t_vertex){map->player.anglecos, map->player.anglesin},
		sect->vertices[s], sect->vertices[s+1]);
	if (d > 0 && d < 3)
		r->t1.x  = maxf(fabs(r->t1.x), 0.1f);
}

static void setup_wall_texture(t_main *m, t_wall *w, int s)
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
		if (r->t1.z < 0 && r->t2.z < 0)
			continue;
		if (r->t1.z <= 0 || r->t2.z <= 0)
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

