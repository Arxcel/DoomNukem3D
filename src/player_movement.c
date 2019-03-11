/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 13:29:55 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 15:21:26 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		transform_player(t_map *map)
{

	t_sector * sect;
	t_vertex * vert;
	int s;

	sect = &map->sectors[map->player.sector_number];
	vert = sect->vertices;
	s = -1;
	while (++s < sect->number_vertices)
		if(sect->neighbors[s] >= 0 &&
		intersects(&map->player, vert[s], vert[s + 1]))
		{
			map->player.sector_number = sect->neighbors[s];
			break;
		}
	map->player.position.x += map->player.velocity.x;
	map->player.position.y += map->player.velocity.y;
	map->player.position.z += map->player.velocity.z;
}

static void player_vertical_movement(t_main *m)
{
    float		eyeheight;
    float		nextz;
	t_sector	*player_sector;

	player_sector = &m->map.sectors[m->map.player.sector_number];
    eyeheight = m->map.player.is_crouching ? CROUCHINGHEIGHT : STANDHEIGHT;
    m->map.player.velocity.z -= 0.01f;
    nextz = m->map.player.position.z + m->map.player.velocity.z;
    if (m->map.player.velocity.z < 0 &&
							nextz < player_sector->floor_height + eyeheight)
    {
        m->map.player.position.z = player_sector->floor_height + eyeheight;
        m->map.player.velocity.z = 0;
        m->map.player.is_falling  = 0;
        m->map.player.is_standing = 1;
    }
    else if (m->map.player.velocity.z > 0 && nextz > player_sector->ceil_height)
    {
        m->map.player.velocity.z = 0;
        m->map.player.is_falling = 1;
    }
}

static void player_horizontal_movement(t_main *m)
{
	t_sector * sect;
	t_vertex * vert;
	int s;
	float hole_low;
	float hole_high;

	sect = &m->map.sectors[m->map.player.sector_number];
	vert = sect->vertices;
	s = -1;
	while (++s < sect->number_vertices)
		if(intersects(&m->map.player, vert[s], vert[s + 1]))
		{
			hole_low  = sect->neighbors[s] < 0 ? -1 : maxf(sect->floor_height,
							m->map.sectors[sect->neighbors[s]].floor_height);
			hole_high = sect->neighbors[s] < 0 ? -1 : minf(sect->ceil_height,
							m->map.sectors[sect->neighbors[s]].ceil_height);
			if(hole_high < m->map.player.position.z + HEADMARGIN
			|| hole_low >
			m->map.player.position.z - (m->map.player.is_crouching ?
								CROUCHINGHEIGHT : STANDHEIGHT) + KNEEHEIGHT)
			{
				m->map.player.velocity.x = 0;
				m->map.player.velocity.y = 0;
				m->map.player.is_moving = 0;
			}
		}
	transform_player(&m->map);
	m->map.player.is_falling = 1;
}

void move_player(t_main *m)
{
	t_vertex		moveDir;
	int				pushing;
	float			acceleration;

	m->map.player.is_standing = !m->map.player.is_falling;
	if (m->map.player.is_falling)
		player_vertical_movement(m);
	if (m->map.player.is_moving)
		player_horizontal_movement(m);
	get_player_direction(m);
	moveDir = get_player_velocity(&m->map.player);
	pushing = m->map.player.dir.forward || m->map.player.dir.backward ||
	m->map.player.dir.right || m->map.player.dir.left || m->map.player.is_falling;
	acceleration = pushing ? 0.4 : 0.2;
	m->map.player.velocity.x = m->map.player.velocity.x *
					(1 - acceleration) + moveDir.x * acceleration;
	m->map.player.velocity.y = m->map.player.velocity.y *
					(1 - acceleration) + moveDir.y * acceleration;
	if(pushing)
		m->map.player.is_moving = 1;
}

