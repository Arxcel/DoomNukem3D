/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_player_movement.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 13:29:55 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 13:33:03 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		transform_player(t_map *map, float dx, float dy)
{	
	float px = map->player.position.x;
	float py = map->player.position.y;

	t_sector const* sect = &map->sectors[map->player.sector_number];
	t_vertex const* vert = sect->vertices;
	int s = -1;
	while (++s < sect->number_vertices)
		if (sect->neighbors[s] >= 0
		&& intersect_box((t_vertex){px, py}, (t_vertex){px + dx, py + dy},(t_vertex){vert[s + 0].x, vert[s + 0].y}, (t_vertex){vert[s + 1].x, vert[s + 1].y})
		&& point_side((t_vertex){px + dx, py + dy}, (t_vertex){vert[s + 0].x, vert[s + 0].y}, (t_vertex){vert[s + 1].x, vert[s + 1].y}) < 0)
		{
			map->player.sector_number = sect->neighbors[s];
			break;
		}
	map->player.position.x += dx;
	map->player.position.y += dy;
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

static void		player_horizontal_movement(t_main *m)
{
	float px = m->map.player.position.x;
	float py = m->map.player.position.y;
	float dx = m->map.player.velocity.x;
	float dy = m->map.player.velocity.y;
	float eyeheight = m->map.player.is_crouching ? CROUCHINGHEIGHT : STANDHEIGHT;
	t_sector *sect = &m->map.sectors[m->map.player.sector_number];
	t_vertex *vert = sect->vertices;

	/* Check if the player is about to cross one of the sector's edges */
	int s = -1;
	while (++s < sect->number_vertices)
		if (intersect_box((t_vertex){px, py}, (t_vertex){px + dx, py + dy}, (t_vertex){vert[s + 0].x, vert[s + 0].y}, (t_vertex){vert[s + 1].x, vert[s + 1].y})
		&& point_side((t_vertex){px + dx, py + dy}, (t_vertex){vert[s + 0].x, vert[s + 0].y}, (t_vertex){vert[s + 1].x, vert[s + 1].y}) < 0)
		{
			/* Check where the hole is. */
			float hole_low  = sect->neighbors[s] < 0 ? -1 : maxf(sect->floor_height, m->map.sectors[sect->neighbors[s]].floor_height);
			float hole_high = sect->neighbors[s] < 0 ? -1 : minf(sect->ceil_height,  m->map.sectors[sect->neighbors[s]].ceil_height );
			/* Check whether we're bumping into a wall. */
			if(hole_high < m->map.player.position.z + HEADMARGIN
			|| hole_low  > m->map.player.position.z - eyeheight + KNEEHEIGHT)
			{
				dx = 0;
				dy = 0;
				m->map.player.is_moving = 0;
			}
		}
	transform_player(&m->map, dx, dy);
	m->map.player.is_falling = 1;
}

static t_vertex	get_player_direction(t_main *m)
{
	int x;
	int y;
	float pitch;
	t_vertex moveDir;

	pitch = 0;
	ft_bzero(&moveDir, sizeof(t_vertex));
	SDL_GetRelativeMouseState(&x, &y);
	m->map.player.angle += x * 0.03f;
	pitch = clampf(pitch + y * 0.05f, -5, 5);
	m->map.player.pitch += pitch- m->map.player.velocity.z * 0.1f;
	transform_player(&m->map, 0, 0);
	if(m->map.player.dir == Forward)
	{
		moveDir.x += m->map.player.anglecos * 0.1f;
		moveDir.y += m->map.player.anglesin * 0.1f;
	}
	if(m->map.player.dir == Backward)
	{
		moveDir.x -= m->map.player.anglecos * 0.1f;
		moveDir.y -= m->map.player.anglesin * 0.1f;
	}
	if(m->map.player.dir == Left)
	{
		moveDir.x += m->map.player.anglesin * 0.1f;
		moveDir.y -= m->map.player.anglecos * 0.1f;
	}
	if(m->map.player.dir == Right)
	{
		moveDir.x -= m->map.player.anglesin * 0.1f;
		moveDir.y += m->map.player.anglecos * 0.1f;
	}
	return moveDir;
}

void			move_player(t_main *m)
{
	t_vertex moveDir;
	int pushing;
	float acceleration;

	m->map.player.is_standing = !m->map.player.is_falling;
	if (m->map.player.is_falling)
		player_vertical_movement(m);
	if (m->map.player.is_moving)
		player_horizontal_movement(m);
	moveDir = get_player_direction(m);
	pushing = m->map.player.dir;
	acceleration = pushing ? 0.4 : 0.2;
	m->map.player.velocity.x = m->map.player.velocity.x *
							(1 - acceleration) + moveDir.x * acceleration;
	m->map.player.velocity.y = m->map.player.velocity.y *
							(1 - acceleration) + moveDir.y * acceleration;
	if (pushing)
		m->map.player.is_moving = 1;
}
