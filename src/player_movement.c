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

static int intersects(t_player const *p, t_vertex const *v1, t_vertex const *v2)
{
	t_vertex p_old_pos;
	t_vertex p_new_pos;

	p_old_pos = (t_vertex) { p->position.x, p->position.y };
	p_new_pos.x = p->position.x + p->velocity.x;
	p_new_pos.y = p->position.y + p->velocity.y;
	return (intersect_box(p_old_pos, p_new_pos, *v1, *v2) && point_side(p_new_pos, *v1, *v2) < 0);
}

void		transform_player(t_map *map)
{	
	float speed;

	speed = 0.1f;
	if (map->player.is_moving)
	{
		map->player.position.x += map->player.velocity.x * speed;
		map->player.position.y += map->player.velocity.y * speed;
	}
	if (!map->player.is_grounded)
	{
		map->player.position.z += map->player.velocity.z;
	}
}

static void		player_vertical_movement(t_main *m)
{
	float		eyeheight;
	float		nextz;
	float		gravity;

	gravity = 0.01f;
	eyeheight = m->map.player.is_crouching ? CROUCHINGHEIGHT : STANDHEIGHT;
	m->map.player.velocity.z -= gravity;
	nextz = m->map.player.position.z + m->map.player.velocity.z;
	if (m->map.player.velocity.z < 0 && nextz < m->map.sectors[m->map.player.sector_number].floor_height + eyeheight)
	{
		m->map.player.position.z = m->map.sectors[m->map.player.sector_number].floor_height + eyeheight;
		m->map.player.velocity.z = 0;
		m->map.player.is_grounded = 1;
	}
	else if (m->map.player.velocity.z > 0 && nextz > m->map.sectors[m->map.player.sector_number].ceil_height)
	{
		m->map.player.velocity.z = 0;
		m->map.player.is_grounded = 0;
	}
}

static void		player_horizontal_movement(t_main *m)
{
	t_sector	*sect;
	float		eyeheight;
	float		hole_low;
	float		hole_high;
	int			s;

	s = -1;
	eyeheight = m->map.player.is_crouching ? CROUCHINGHEIGHT : STANDHEIGHT;
	sect = &m->map.sectors[m->map.player.sector_number];
	while (++s < sect->number_vertices)
		if (intersects(&m->map.player, &sect->vertices[s], &sect->vertices[s + 1]))
		{
			hole_low  = sect->neighbors[s] < 0 ? -1 : maxf(sect->floor_height, m->map.sectors[sect->neighbors[s]].floor_height);
			hole_high = sect->neighbors[s] < 0 ? -1 : minf(sect->ceil_height,  m->map.sectors[sect->neighbors[s]].ceil_height );
			if (sect->neighbors[s] >= 0 && hole_high >= m->map.player.position.z + HEADMARGIN && hole_low <= m->map.player.position.z - eyeheight + KNEEHEIGHT)
			{
				m->map.player.sector_number = sect->neighbors[s];
				m->map.player.is_moving = 1;
			}
			else
			{
				m->map.player.is_moving = 0;
			}
			m->map.player.is_grounded = 0;
			break;
		}
}

static void		get_player_velocity(t_player *p)
{
	p->is_moving = p->dir;
	if (p->dir == Forward)
	{
		p->velocity.x += p->anglecos;
		p->velocity.y += p->anglesin;
	}
	if (p->dir == Backward)
	{
		p->velocity.x -= p->anglecos;
		p->velocity.y -= p->anglesin;
	}
	if (p->dir == Left)
	{
		p->velocity.x += p->anglesin;
		p->velocity.y -= p->anglecos;
	}
	if (p->dir == Right)
	{
		p->velocity.x -= p->anglesin;
		p->velocity.y += p->anglecos;
	}
}

void			move_player(t_main *m)
{
	t_vertex		velocity;

	m->map.player.velocity.x = 0.f;
	m->map.player.velocity.y = 0.f;
	get_player_direction(m);
	get_player_velocity(&m->map.player);
	player_vertical_movement(m);
	player_horizontal_movement(m);
	transform_player(&m->map);
}

