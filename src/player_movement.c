/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 13:29:55 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/29 14:17:32 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void			transform_player(t_map *map)
{
	t_sector	*sect;
	t_vertex	*vert;
	int			s;

	sect = &map->sectors[map->player.sector_number];
	vert = sect->vertices;
	s = -1;
	while (++s < sect->number_vertices)
		if (sect->neighbors[s] >= 0 &&
			intersects(&map->player, vert[s], vert[s + 1]))
		{
			map->player.sector_number = sect->neighbors[s];
			break ;
		}
	map->player.position.x += map->player.velocity.x;
	map->player.position.y += map->player.velocity.y;
	map->player.position.z += map->player.velocity.z;
}

static void			player_vertical_movement(t_main *m)
{
	float		eyeheight;
	float		nextz;
	t_sector	*player_sector;

	player_sector = &m->map.sectors[m->map.player.sector_number];
	eyeheight = m->map.player.is_crouching ? CROUCHINGHEIGHT : STANDHEIGHT;
	m->map.player.velocity.z -= m->delta_time * m->map.player.gravity;
	nextz = m->map.player.position.z + m->map.player.velocity.z;
	if (m->map.player.velocity.z < 0 &&
							nextz < player_sector->floor_height + eyeheight)
	{
		m->map.player.position.z = player_sector->floor_height + eyeheight;
		m->map.player.velocity.z = 0;
		m->map.player.is_falling = false;
		m->map.player.is_standing = true;
	}
	else if (m->map.player.velocity.z > 0 && nextz > player_sector->ceil_height)
	{
		m->map.player.velocity.z = 0;
		m->map.player.is_falling = true;
	}
}

static void			update_velocity(t_player *p, t_vertex p1, t_vertex p2)
{
	float xd;
	float yd;

	xd = p2.x - p1.x;
	yd = p2.y - p1.y;
	p->velocity.x = xd * (p->velocity.x * xd + p->velocity.y * yd) /
													(xd * xd + yd * yd);
	p->velocity.y = yd * (p->velocity.x * xd + p->velocity.y * yd) /
													(xd * xd + yd * yd);
}

static void			player_horizontal_movement(t_main *m)
{
	t_sector	*sect;
	int			s;
	t_vertex	hole;

	sect = &m->map.sectors[m->map.player.sector_number];
	s = -1;
	while (++s < sect->number_vertices)
		if (intersects(&m->map.player, sect->vertices[s],
										sect->vertices[s + 1]))
		{
			hole.x = sect->neighbors[s] < 0 ? -1 : maxf(sect->floor_height,
							m->map.sectors[sect->neighbors[s]].floor_height);
			hole.y = sect->neighbors[s] < 0 ? -1 : minf(sect->ceil_height,
							m->map.sectors[sect->neighbors[s]].ceil_height);
			if (hole.y < m->map.player.position.z + HEADMARGIN || hole.x >
			m->map.player.position.z - (m->map.player.is_crouching ?
			CROUCHINGHEIGHT : STANDHEIGHT) + KNEEHEIGHT)
			{
				update_velocity(&m->map.player, sect->vertices[s],
													sect->vertices[s + 1]);
				m->map.player.is_moving = false;
			}
		}
	transform_player(&m->map);
	m->map.player.is_falling = true;
}

void				move_player(t_main *m)
{
	t_vertex		move_dir;
	bool			pushing;
	float			acceleration;

	m->map.player.is_standing = !m->map.player.is_falling;
	if (m->map.player.is_falling)
		player_vertical_movement(m);
	if (m->map.player.is_moving)
		player_horizontal_movement(m);
	get_player_direction(m);
	move_dir = get_player_velocity(m);
	pushing = m->map.player.dir.forward || m->map.player.dir.backward ||
				m->map.player.dir.right || m->map.player.dir.left ||
				m->map.player.is_falling;
	acceleration = pushing ? 0.4 : 0.2;
	m->map.player.velocity.x = m->map.player.velocity.x *
					(1 - acceleration) + move_dir.x * acceleration;
	m->map.player.velocity.y = m->map.player.velocity.y *
					(1 - acceleration) + move_dir.y * acceleration;
	if (pushing)
		m->map.player.is_moving = true;
}
