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
	t_sector const* sect = &map->sectors[map->player.sector_number];
	t_vertex const* vert = sect->vertices;
	int s = -1;
	while (++s < sect->number_vertices)
		if (sect->neighbors[s] >= 0
		&& intersects(&map->player, &sect->vertices[s], &sect->vertices[s + 1]))
		{
			map->player.sector_number = sect->neighbors[s];
			break;
		}
	map->player.position.x += map->player.velocity.x;
	map->player.position.y += map->player.velocity.y;
	map->player.anglesin = sinf(map->player.angle);
	map->player.anglecos = cosf(map->player.angle);
}

static void		player_vertical_movement(t_main *m)
{
	float eyeheight;
	float nextz;

	eyeheight = m->map.player.is_crouching ? CROUCHINGHEIGHT : STANDHEIGHT;
	m->map.player.velocity.z -= 0.01f;
	nextz = m->map.player.position.z + m->map.player.velocity.z;
	if (m->map.player.velocity.z < 0 && nextz  < m->map.sectors[m->map.player.sector_number].floor_height + eyeheight)
	{
		m->map.player.position.z = m->map.sectors[m->map.player.sector_number].floor_height + eyeheight;
		m->map.player.velocity.z = 0;
		m->map.player.is_falling  = 0;
		m->map.player.is_standing = 1;
	}
	else if (m->map.player.velocity.z > 0 && nextz > m->map.sectors[m->map.player.sector_number].ceil_height)
	{
		m->map.player.velocity.z = 0;
		m->map.player.is_falling = 1;
	}
	if (m->map.player.is_falling)
	{
		m->map.player.position.z += m->map.player.velocity.z;
		m->map.player.is_moving = 1;
	}
}

static void		checkCrossingEdges()
{

}

static void		player_horizontal_movement(t_main *m)
{
	float eyeheight = m->map.player.is_crouching ? CROUCHINGHEIGHT : STANDHEIGHT;
	t_sector *sect = &m->map.sectors[m->map.player.sector_number];
	t_vertex *vert = sect->vertices;
	int is_bumping = 0;

	/* Check if the player is about to cross one of the sector's edges */
	int s = -1;
	while (++s < sect->number_vertices)
		if (intersect_box((t_vertex){m->map.player.position.x, m->map.player.position.y}, (t_vertex){m->map.player.position.x + m->map.player.velocity.x, m->map.player.position.y + m->map.player.velocity.y}, vert[s], vert[s + 1])
		&& point_side((t_vertex){m->map.player.position.x + m->map.player.velocity.x, m->map.player.position.y + m->map.player.velocity.y}, vert[s + 0], vert[s + 1]) < 0)
		{
			float hole_low  = sect->neighbors[s] < 0 ? -1 : maxf(sect->floor_height, m->map.sectors[sect->neighbors[s]].floor_height);
			float hole_high = sect->neighbors[s] < 0 ? -1 : minf(sect->ceil_height,  m->map.sectors[sect->neighbors[s]].ceil_height );
			if(hole_high < m->map.player.position.z + HEADMARGIN
			|| hole_low  > m->map.player.position.z - eyeheight + KNEEHEIGHT)
			{
				m->map.player.velocity.x = 0;
				m->map.player.velocity.y = 0;
				m->map.player.is_moving = 0;
				is_bumping = 1;
			}
		}
	if(is_bumping)
		transform_player(&m->map);
	m->map.player.is_falling = 1;
}

void			move_player(t_main *m)
{
	t_vertex		move_dir;
	int				pushing;
	float			acceleration;

	m->map.player.is_standing = !m->map.player.is_falling;
	if (m->map.player.is_falling)
		player_vertical_movement(m);
	if (m->map.player.is_moving)
		player_horizontal_movement(m);
	move_dir = get_player_direction(m);
	pushing = m->map.player.dir;
	acceleration = pushing ? 0.4 : 0.2;
	m->map.player.velocity.x = m->map.player.velocity.x *
							(1 - acceleration) + move_dir.x * acceleration;
	m->map.player.velocity.y = m->map.player.velocity.y *
							(1 - acceleration) + move_dir.y * acceleration;
	if (pushing)
		m->map.player.is_moving = 1;
}

